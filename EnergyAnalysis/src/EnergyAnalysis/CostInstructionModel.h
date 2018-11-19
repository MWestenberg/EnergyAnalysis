#pragma once
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/IR/LegacyPassManagers.h"
#include "EnergyAnalysis.h"
#include "Constants.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/Analysis/VectorUtils.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Support/CommandLine.h"

class CostInstructionModel : public llvm::FunctionPass, public llvm::PMDataManager, public llvm::PMTopLevelManager
{
public:
	static char ID;
	explicit CostInstructionModel() : llvm::FunctionPass(ID), llvm::PMTopLevelManager(new llvm::FPPassManager()), m_F(nullptr), TTI(nullptr) {}
	bool runOnFunction(llvm::Function& F);
	unsigned getInstructionCost(const llvm::Instruction* I) const;

private:
	llvm::Function* m_F;
	const llvm::TargetTransformInfo *TTI;


	// Inherited via PMDataManager
	virtual llvm::Pass * getAsPass() override;

	// Inherited via PMTopLevelManager
	virtual llvm::PMDataManager * getAsPMDataManager() override;

	virtual llvm::PassManagerType getTopLevelPassManagerType() override;
	
	void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;

	//static helper methods
	static llvm::TargetTransformInfo::OperandValueKind getOperandInfo(llvm::Value *V);
	static bool matchVectorSplittingReduction(const llvm::ExtractElementInst *ReduxRoot, unsigned &Opcode, llvm::Type *&Ty);
	static bool matchPairwiseReductionAtLevel(const llvm::BinaryOperator *BinOp, unsigned Level, unsigned NumLevels);
	static bool matchPairwiseShuffleMask(llvm::ShuffleVectorInst *SI, bool IsLeft, unsigned Level);
	static bool matchPairwiseReduction(const llvm::ExtractElementInst *ReduxRoot, unsigned &Opcode, llvm::Type *&Ty);

	static std::pair<llvm::Value *, llvm::ShuffleVectorInst *> getShuffleAndOtherOprd(llvm::BinaryOperator *B);
	static bool isReverseVectorMask(llvm::ArrayRef<int> Mask);
	static bool isSingleSourceVectorMask(llvm::ArrayRef<int> Mask);
	static bool isZeroEltBroadcastVectorMask(llvm::ArrayRef<int> Mask);
	static bool isAlternateVectorMask(llvm::ArrayRef<int> Mask);
};

