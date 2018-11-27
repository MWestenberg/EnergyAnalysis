#pragma once

#include "EnergyAnalysis.h"
#include "Constants.h"

#include "llvm/IR/LegacyPassManagers.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/InlineCost.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/Analysis/TargetTransformInfo.h"


class WCETAnalysisPass : public llvm::FunctionPass, public llvm::PMDataManager, public llvm::PMTopLevelManager, public Analysis
{

private: 
	llvm::Function* F;
	const llvm::TargetTransformInfo* TTI;

	const static bool EnableReduxCost = true;
public:

	static char ID;

	WCETAnalysisPass() : llvm::FunctionPass(ID), PMDataManager(), PMTopLevelManager(new llvm::FPPassManager()), F(nullptr)
	{
		llvm::initializeTargetTransformInfoWrapperPassPass(*llvm::PassRegistry::getPassRegistry());
		llvm::initializeLoopInfoWrapperPassPass(*llvm::PassRegistry::getPassRegistry());
		llvm::initializeScalarEvolutionWrapperPassPass(*llvm::PassRegistry::getPassRegistry());
		llvm::initializeCostModelAnalysisPass(	*llvm::PassRegistry::getPassRegistry());
	}

	//Override of PMDataManager
	llvm::PMDataManager *getAsPMDataManager() override { return this; }

	// Override of Pass
	llvm::Pass *getAsPass() override { return llvm::createCostModelAnalysisPass(); }

	//Override of PMTOPLevelManager
	llvm::PassManagerType getTopLevelPassManagerType() override {
		return llvm::PMT_BasicBlockPassManager;
	}

	// Inherited via FunctionPass
	virtual bool runOnFunction(llvm::Function & F) override;

	unsigned getInstructionCost(const llvm::Instruction *I) const;

	void print(llvm::raw_ostream &OS, const llvm::Module*) const;
	~WCETAnalysisPass() {
		std::cout << "GODVERDOMME!!!!!!" << std::endl;
	}

private:

	// LLVM Anaysis usage method to preserve pass data
	void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;


	//BELOW SOME TEST


	static bool isReverseVectorMask(llvm::SmallVectorImpl<int> &Mask) {
		for (unsigned i = 0, MaskSize = Mask.size(); i < MaskSize; ++i)
			if (Mask[i] > 0 && Mask[i] != (int)(MaskSize - 1 - i))
				return false;
		return true;
	}

	static bool isAlternateVectorMask(llvm::SmallVectorImpl<int> &Mask) {
		bool isAlternate = true;
		unsigned MaskSize = Mask.size();

		// Example: shufflevector A, B, <0,5,2,7>
		for (unsigned i = 0; i < MaskSize && isAlternate; ++i) {
			if (Mask[i] < 0)
				continue;
			isAlternate = Mask[i] == (int)((i & 1) ? MaskSize + i : i);
		}

		if (isAlternate)
			return true;

		isAlternate = true;
		// Example: shufflevector A, B, <4,1,6,3>
		for (unsigned i = 0; i < MaskSize && isAlternate; ++i) {
			if (Mask[i] < 0)
				continue;
			isAlternate = Mask[i] == (int)((i & 1) ? i : MaskSize + i);
		}

		return isAlternate;
	}

	static llvm::TargetTransformInfo::OperandValueKind getOperandInfo(llvm::Value *V) {
		llvm::TargetTransformInfo::OperandValueKind OpInfo =
			llvm::TargetTransformInfo::OK_AnyValue;

		// Check for a splat of a constant or for a non uniform vector of constants.
		if (llvm::isa<llvm::ConstantVector>(V) || llvm::isa<llvm::ConstantDataVector>(V)) {
			OpInfo = llvm::TargetTransformInfo::OK_NonUniformConstantValue;
			if (llvm::cast<llvm::Constant>(V)->getSplatValue() != nullptr)
				OpInfo = llvm::TargetTransformInfo::OK_UniformConstantValue;
		}

		return OpInfo;
	}

	static bool matchPairwiseShuffleMask(llvm::ShuffleVectorInst *SI, bool IsLeft,
		unsigned Level) {
		// We don't need a shuffle if we just want to have element 0 in position 0 of
		// the vector.
		if (!SI && Level == 0 && IsLeft)
			return true;
		else if (!SI)
			return false;

		llvm::SmallVector<int, 32> Mask(SI->getType()->getVectorNumElements(), -1);

		// Build a mask of 0, 2, ... (left) or 1, 3, ... (right) depending on whether
		// we look at the left or right side.
		for (unsigned i = 0, e = (1 << Level), val = !IsLeft; i != e; ++i, val += 2)
			Mask[i] = val;

		llvm::SmallVector<int, 16> ActualMask = SI->getShuffleMask();
		if (Mask != ActualMask)
			return false;

		return true;
	}

	static bool matchPairwiseReductionAtLevel(const llvm::BinaryOperator *BinOp,
		unsigned Level, unsigned NumLevels) {
		// Match one level of pairwise operations.
		// %rdx.shuf.0.0 = shufflevector <4 x float> %rdx, <4 x float> undef,
		//       <4 x i32> <i32 0, i32 2 , i32 undef, i32 undef>
		// %rdx.shuf.0.1 = shufflevector <4 x float> %rdx, <4 x float> undef,
		//       <4 x i32> <i32 1, i32 3, i32 undef, i32 undef>
		// %bin.rdx.0 = fadd <4 x float> %rdx.shuf.0.0, %rdx.shuf.0.1
		if (BinOp == nullptr)
			return false;

		assert(BinOp->getType()->isVectorTy() && "Expecting a vector type");

		unsigned Opcode = BinOp->getOpcode();
		llvm::Value *L = BinOp->getOperand(0);
		llvm::Value *R = BinOp->getOperand(1);

		llvm::ShuffleVectorInst *LS = llvm::dyn_cast<llvm::ShuffleVectorInst>(L);
		if (!LS && Level)
			return false;
		llvm::ShuffleVectorInst *RS = llvm::dyn_cast<llvm::ShuffleVectorInst>(R);
		if (!RS && Level)
			return false;

		// On level 0 we can omit one shufflevector instruction.
		if (!Level && !RS && !LS)
			return false;

		// Shuffle inputs must match.
		llvm::Value *NextLevelOpL = LS ? LS->getOperand(0) : nullptr;
		llvm::Value *NextLevelOpR = RS ? RS->getOperand(0) : nullptr;
		llvm::Value *NextLevelOp = nullptr;
		if (NextLevelOpR && NextLevelOpL) {
			// If we have two shuffles their operands must match.
			if (NextLevelOpL != NextLevelOpR)
				return false;

			NextLevelOp = NextLevelOpL;
		}
		else if (Level == 0 && (NextLevelOpR || NextLevelOpL)) {
			// On the first level we can omit the shufflevector <0, undef,...>. So the
			// input to the other shufflevector <1, undef> must match with one of the
			// inputs to the current binary operation.
			// Example:
			//  %NextLevelOpL = shufflevector %R, <1, undef ...>
			//  %BinOp        = fadd          %NextLevelOpL, %R
			if (NextLevelOpL && NextLevelOpL != R)
				return false;
			else if (NextLevelOpR && NextLevelOpR != L)
				return false;

			NextLevelOp = NextLevelOpL ? R : L;
		}
		else
			return false;

		// Check that the next levels binary operation exists and matches with the
		// current one.
		llvm::BinaryOperator *NextLevelBinOp = nullptr;
		if (Level + 1 != NumLevels) {
			if (!(NextLevelBinOp = llvm::dyn_cast<llvm::BinaryOperator>(NextLevelOp)))
				return false;
			else if (NextLevelBinOp->getOpcode() != Opcode)
				return false;
		}

		// Shuffle mask for pairwise operation must match.
		if (matchPairwiseShuffleMask(LS, true, Level)) {
			if (!matchPairwiseShuffleMask(RS, false, Level))
				return false;
		}
		else if (matchPairwiseShuffleMask(RS, true, Level)) {
			if (!matchPairwiseShuffleMask(LS, false, Level))
				return false;
		}
		else
			return false;

		if (++Level == NumLevels)
			return true;

		// Match next level.
		return matchPairwiseReductionAtLevel(NextLevelBinOp, Level, NumLevels);
	}

	static bool matchPairwiseReduction(const llvm::ExtractElementInst *ReduxRoot,
		unsigned &Opcode, llvm::Type *&Ty) {
		if (!EnableReduxCost)
			return false;

		// Need to extract the first element.
		llvm::ConstantInt *CI = llvm::dyn_cast<llvm::ConstantInt>(ReduxRoot->getOperand(1));
		unsigned Idx = ~0u;
		if (CI)
			Idx = CI->getZExtValue();
		if (Idx != 0)
			return false;

		llvm::BinaryOperator *RdxStart = llvm::dyn_cast<llvm::BinaryOperator>(ReduxRoot->getOperand(0));
		if (!RdxStart)
			return false;

		llvm::Type *VecTy = ReduxRoot->getOperand(0)->getType();
		unsigned NumVecElems = VecTy->getVectorNumElements();
		if (!llvm::isPowerOf2_32(NumVecElems))
			return false;

		// We look for a sequence of shuffle,shuffle,add triples like the following
		// that builds a pairwise reduction tree.
		//
		//  (X0, X1, X2, X3)
		//   (X0 + X1, X2 + X3, undef, undef)
		//    ((X0 + X1) + (X2 + X3), undef, undef, undef)
		//
		// %rdx.shuf.0.0 = shufflevector <4 x float> %rdx, <4 x float> undef,
		//       <4 x i32> <i32 0, i32 2 , i32 undef, i32 undef>
		// %rdx.shuf.0.1 = shufflevector <4 x float> %rdx, <4 x float> undef,
		//       <4 x i32> <i32 1, i32 3, i32 undef, i32 undef>
		// %bin.rdx.0 = fadd <4 x float> %rdx.shuf.0.0, %rdx.shuf.0.1
		// %rdx.shuf.1.0 = shufflevector <4 x float> %bin.rdx.0, <4 x float> undef,
		//       <4 x i32> <i32 0, i32 undef, i32 undef, i32 undef>
		// %rdx.shuf.1.1 = shufflevector <4 x float> %bin.rdx.0, <4 x float> undef,
		//       <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
		// %bin.rdx8 = fadd <4 x float> %rdx.shuf.1.0, %rdx.shuf.1.1
		// %r = extractelement <4 x float> %bin.rdx8, i32 0
		if (!matchPairwiseReductionAtLevel(RdxStart, 0, llvm::Log2_32(NumVecElems)))
			return false;

		Opcode = RdxStart->getOpcode();
		Ty = VecTy;

		return true;
	}

	static std::pair<llvm::Value *, llvm::ShuffleVectorInst *>
		getShuffleAndOtherOprd(llvm::BinaryOperator *B) {

		llvm::Value *L = B->getOperand(0);
		llvm::Value *R = B->getOperand(1);
		llvm::ShuffleVectorInst *S = nullptr;

		if ((S = llvm::dyn_cast<llvm::ShuffleVectorInst>(L)))
			return std::make_pair(R, S);

		S = llvm::dyn_cast<llvm::ShuffleVectorInst>(R);
		return std::make_pair(L, S);
	}

	static bool matchVectorSplittingReduction(const llvm::ExtractElementInst *ReduxRoot,
		unsigned &Opcode, llvm::Type *&Ty) {
		if (!EnableReduxCost)
			return false;

		// Need to extract the first element.
		llvm::ConstantInt *CI = llvm::dyn_cast<llvm::ConstantInt>(ReduxRoot->getOperand(1));
		unsigned Idx = ~0u;
		if (CI)
			Idx = CI->getZExtValue();
		if (Idx != 0)
			return false;

		llvm::BinaryOperator *RdxStart = llvm::dyn_cast<llvm::BinaryOperator>(ReduxRoot->getOperand(0));
		if (!RdxStart)
			return false;
		unsigned RdxOpcode = RdxStart->getOpcode();

		llvm::Type *VecTy = ReduxRoot->getOperand(0)->getType();
		unsigned NumVecElems = VecTy->getVectorNumElements();
		if (!llvm::isPowerOf2_32(NumVecElems))
			return false;

		// We look for a sequence of shuffles and adds like the following matching one
		// fadd, shuffle vector pair at a time.
		//
		// %rdx.shuf = shufflevector <4 x float> %rdx, <4 x float> undef,
		//                           <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
		// %bin.rdx = fadd <4 x float> %rdx, %rdx.shuf
		// %rdx.shuf7 = shufflevector <4 x float> %bin.rdx, <4 x float> undef,
		//                          <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
		// %bin.rdx8 = fadd <4 x float> %bin.rdx, %rdx.shuf7
		// %r = extractelement <4 x float> %bin.rdx8, i32 0

		unsigned MaskStart = 1;
		llvm::Value *RdxOp = RdxStart;
		llvm::SmallVector<int, 32> ShuffleMask(NumVecElems, 0);
		unsigned NumVecElemsRemain = NumVecElems;
		while (NumVecElemsRemain - 1) {
			// Check for the right reduction operation.
			llvm::BinaryOperator *BinOp;
			if (!(BinOp = llvm::dyn_cast<llvm::BinaryOperator>(RdxOp)))
				return false;
			if (BinOp->getOpcode() != RdxOpcode)
				return false;

			llvm::Value *NextRdxOp;
			llvm::ShuffleVectorInst *Shuffle;
			std::tie(NextRdxOp, Shuffle) = getShuffleAndOtherOprd(BinOp);

			// Check the current reduction operation and the shuffle use the same value.
			if (Shuffle == nullptr)
				return false;
			if (Shuffle->getOperand(0) != NextRdxOp)
				return false;

			// Check that shuffle masks matches.
			for (unsigned j = 0; j != MaskStart; ++j)
				ShuffleMask[j] = MaskStart + j;
			// Fill the rest of the mask with -1 for undef.
			std::fill(&ShuffleMask[MaskStart], ShuffleMask.end(), -1);

			llvm::SmallVector<int, 16> Mask = Shuffle->getShuffleMask();
			if (ShuffleMask != Mask)
				return false;

			RdxOp = NextRdxOp;
			NumVecElemsRemain /= 2;
			MaskStart *= 2;
		}

		Opcode = RdxOpcode;
		Ty = VecTy;
		return true;
	}




};

