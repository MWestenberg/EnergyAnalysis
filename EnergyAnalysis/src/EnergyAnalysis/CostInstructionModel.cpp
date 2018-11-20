#include "CostInstructionModel.h"

////REGISTER THE PASS
char CostInstructionModel::ID = 22;


bool CostInstructionModel::runOnFunction(llvm::Function & F)
{
	m_F = &F;
	llvm::PMDataManager *DM = getAsPMDataManager();
	llvm::AnalysisResolver *AR = new llvm::AnalysisResolver(*DM);
	setResolver(AR);
	setTopLevelManager(new CostInstructionModel());

	recordAvailableAnalysis(new llvm::TargetTransformInfoWrapperPass());
	auto *TTIWP = getAnalysisIfAvailable<llvm::TargetTransformInfoWrapperPass>();
	TTI = TTIWP ? &TTIWP->getTTI(F) : nullptr;

	return false;
}

llvm::Pass * CostInstructionModel::getAsPass()
{
	return this;
}

llvm::PMDataManager * CostInstructionModel::getAsPMDataManager()
{
	return this;
}

llvm::PassManagerType CostInstructionModel::getTopLevelPassManagerType()
{
	return llvm::PMT_BasicBlockPassManager;
}

void CostInstructionModel::getAnalysisUsage(llvm::AnalysisUsage &AU) const {
	AU.setPreservesAll();
}

//static llvm::cl::opt<llvm::TargetTransformInfo::TargetCostKind> CostKind(
//	"cost-kind", llvm::cl::desc("Target cost kind"),
//	llvm::cl::init(llvm::TargetTransformInfo::TCK_RecipThroughput),
//	llvm::cl::values(clEnumValN(llvm::TargetTransformInfo::TCK_RecipThroughput,
//		"throughput", "Reciprocal throughput"),
//		clEnumValN(llvm::TargetTransformInfo::TCK_Latency,
//			"latency", "Instruction latency"),
//		clEnumValN(llvm::TargetTransformInfo::TCK_CodeSize,
//			"code-size", "Code size")));

unsigned CostInstructionModel::GetNewInstructionCost(const llvm::Instruction* I) const
{
	return TTI->getInstructionCost(I, llvm::TargetTransformInfo::TCK_RecipThroughput);
}



unsigned CostInstructionModel::getInstructionCost(const llvm::Instruction* I) const
{
	if (!TTI)
		return -1;


	switch (I->getOpcode()) 
	{
	case llvm::Instruction::GetElementPtr:
		return TTI->getUserCost(I);

	case llvm::Instruction::Ret:
	case llvm::Instruction::PHI:
	case llvm::Instruction::Br: 
		return TTI->getCFInstrCost(I->getOpcode());
	case llvm::Instruction::Add:
	case llvm::Instruction::FAdd:
	case llvm::Instruction::Sub:
	case llvm::Instruction::FSub:
	case llvm::Instruction::Mul:
	case llvm::Instruction::FMul:
	case llvm::Instruction::UDiv:
	case llvm::Instruction::SDiv:
	case llvm::Instruction::FDiv:
	case llvm::Instruction::URem:
	case llvm::Instruction::SRem:
	case llvm::Instruction::FRem:
	case llvm::Instruction::Shl:
	case llvm::Instruction::LShr:
	case llvm::Instruction::AShr:
	case llvm::Instruction::And:
	case llvm::Instruction::Or:
	case llvm::Instruction::Xor: {
		llvm::TargetTransformInfo::OperandValueKind Op1VK =	getOperandInfo(I->getOperand(0));
		llvm::TargetTransformInfo::OperandValueKind Op2VK =	getOperandInfo(I->getOperand(1));
		llvm::SmallVector<const llvm::Value*, 2> Operands(I->operand_values());
		return TTI->getArithmeticInstrCost(I->getOpcode(), I->getType(), Op1VK,	Op2VK, llvm::TargetTransformInfo::OP_None,	llvm::TargetTransformInfo::OP_None,	Operands);
	}
	case llvm::Instruction::Select: {
		const llvm::SelectInst *SI = llvm::cast<llvm::SelectInst>(I);
		llvm::Type *CondTy = SI->getCondition()->getType();
		return TTI->getCmpSelInstrCost(I->getOpcode(), I->getType(), CondTy);
	}
	case llvm::Instruction::ICmp:
	case llvm::Instruction::FCmp: {
		llvm::Type *ValTy = I->getOperand(0)->getType();
		return TTI->getCmpSelInstrCost(I->getOpcode(), ValTy);
	}
	case llvm::Instruction::Store: {
		const llvm::StoreInst *SI = llvm::cast<llvm::StoreInst>(I);
		llvm::Type *ValTy = SI->getValueOperand()->getType();
		return TTI->getMemoryOpCost(I->getOpcode(), ValTy,
			SI->getAlignment(),
			SI->getPointerAddressSpace());
	}
	case llvm::Instruction::Load: {
		const llvm::LoadInst *LI = llvm::cast<llvm::LoadInst>(I);
		return TTI->getMemoryOpCost(I->getOpcode(), I->getType(),
			LI->getAlignment(),
			LI->getPointerAddressSpace());
	}
	case llvm::Instruction::ZExt:
	case llvm::Instruction::SExt:
	case llvm::Instruction::FPToUI:
	case llvm::Instruction::FPToSI:
	case llvm::Instruction::FPExt:
	case llvm::Instruction::PtrToInt:
	case llvm::Instruction::IntToPtr:
	case llvm::Instruction::SIToFP:
	case llvm::Instruction::UIToFP:
	case llvm::Instruction::Trunc:
	case llvm::Instruction::FPTrunc:
	case llvm::Instruction::BitCast:
	case llvm::Instruction::AddrSpaceCast: {
		llvm::Type *SrcTy = I->getOperand(0)->getType();
		return TTI->getCastInstrCost(I->getOpcode(), I->getType(), SrcTy);
	}
	case llvm::Instruction::ExtractElement: {
		const llvm::ExtractElementInst * EEI = llvm::cast<llvm::ExtractElementInst>(I);
		llvm::ConstantInt *CI = llvm::dyn_cast<llvm::ConstantInt>(I->getOperand(1));
		unsigned Idx = -1;
		if (CI)
			Idx = CI->getZExtValue();

		// Try to match a reduction sequence (series of shufflevector and vector
		// adds followed by a extractelement).
		unsigned ReduxOpCode;
		llvm::Type *ReduxType;

		if (matchVectorSplittingReduction(EEI, ReduxOpCode, ReduxType))
			return TTI->getArithmeticReductionCost(ReduxOpCode, ReduxType, false);
		//	return TTI->getReductionCost(ReduxOpCode, ReduxType, false);
		else if (matchPairwiseReduction(EEI, ReduxOpCode, ReduxType))
			return TTI->getArithmeticReductionCost(ReduxOpCode, ReduxType, true);
		//	//return TTI->getReductionCost(ReduxOpCode, ReduxType, true);*/

		return TTI->getVectorInstrCost(I->getOpcode(), EEI->getOperand(0)->getType(), Idx);
	}
	case llvm::Instruction::InsertElement: {
		const llvm::InsertElementInst * IE = llvm::cast<llvm::InsertElementInst>(I);
		llvm::ConstantInt *CI = llvm::dyn_cast<llvm::ConstantInt>(IE->getOperand(2));
		unsigned Idx = -1;
		if (CI)
			Idx = CI->getZExtValue();
		return TTI->getVectorInstrCost(I->getOpcode(),
			IE->getType(), Idx);
	}
	case llvm::Instruction::ShuffleVector: {
		const llvm::ShuffleVectorInst *Shuffle = llvm::cast<llvm::ShuffleVectorInst>(I);
		llvm::Type *VecTypOp0 = Shuffle->getOperand(0)->getType();
		unsigned NumVecElems = VecTypOp0->getVectorNumElements();
		llvm::SmallVector<int, 16> Mask = Shuffle->getShuffleMask();

		if (NumVecElems == Mask.size()) {
			if (isReverseVectorMask(Mask))
				return TTI->getShuffleCost(llvm::TargetTransformInfo::SK_Reverse, VecTypOp0,
					0, nullptr);
			if (isAlternateVectorMask(Mask))
				return TTI->getShuffleCost(llvm::TargetTransformInfo::SK_Select,
					VecTypOp0, 0, nullptr);

			if (isZeroEltBroadcastVectorMask(Mask))
				return TTI->getShuffleCost(llvm::TargetTransformInfo::SK_Broadcast,
					VecTypOp0, 0, nullptr);

			if (isSingleSourceVectorMask(Mask))
				return TTI->getShuffleCost(llvm::TargetTransformInfo::SK_PermuteSingleSrc,
					VecTypOp0, 0, nullptr);

			return TTI->getShuffleCost(llvm::TargetTransformInfo::SK_PermuteTwoSrc,
				VecTypOp0, 0, nullptr);
		}

		return -1;
	}
	case llvm::Instruction::Call:
		if (const llvm::IntrinsicInst *II = llvm::dyn_cast<llvm::IntrinsicInst>(I)) {
			llvm::SmallVector<llvm::Value *, 4> Args;
			for (unsigned J = 0, JE = II->getNumArgOperands(); J != JE; ++J)
				Args.push_back(II->getArgOperand(J));

			llvm::FastMathFlags FMF;
			if (auto *FPMO = llvm::dyn_cast<llvm::FPMathOperator>(II))
				FMF = FPMO->getFastMathFlags();

			return TTI->getIntrinsicInstrCost(II->getIntrinsicID(), II->getType(),
				Args, FMF);
		}
		return -1;
	default:
		// We don't have any information on this instruction.
		return -1;
	}

}


llvm::TargetTransformInfo::OperandValueKind CostInstructionModel::getOperandInfo(llvm::Value *V)
{
	llvm::TargetTransformInfo::OperandValueKind OpInfo = llvm::TargetTransformInfo::OK_AnyValue;

	// Check for a splat of a constant or for a non uniform vector of constants.
	if (llvm::isa<llvm::ConstantVector>(V) || llvm::isa<llvm::ConstantDataVector>(V)) {
		OpInfo = llvm::TargetTransformInfo::OK_NonUniformConstantValue;
		if (llvm::cast<llvm::Constant>(V)->getSplatValue() != nullptr)
			OpInfo = llvm::TargetTransformInfo::OK_UniformConstantValue;
	}

	// Check for a splat of a uniform value. This is not loop aware, so return
	// true only for the obviously uniform cases (argument, globalvalue)
	const llvm::Value *Splat = getSplatValue(V);
	if (Splat && (llvm::isa<llvm::Argument>(Splat) || llvm::isa<llvm::GlobalValue>(Splat)))
		OpInfo = llvm::TargetTransformInfo::OK_UniformValue;

	return OpInfo;
}

static llvm::cl::opt<bool> EnableReduxCost("costmodel-reduxcost-energyanalysis", llvm::cl::init(false), llvm::cl::Hidden, llvm::cl::desc("Recognize reduction patterns."));


bool CostInstructionModel::matchVectorSplittingReduction(const llvm::ExtractElementInst * ReduxRoot, unsigned & Opcode, llvm::Type *& Ty)
{
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

bool CostInstructionModel::matchPairwiseReductionAtLevel(const llvm::BinaryOperator *BinOp, unsigned Level, unsigned NumLevels)
{
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

bool CostInstructionModel::matchPairwiseShuffleMask(llvm::ShuffleVectorInst *SI, bool IsLeft,unsigned Level)
{
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
	return Mask == ActualMask;
}

 bool CostInstructionModel::matchPairwiseReduction(const llvm::ExtractElementInst *ReduxRoot, unsigned &Opcode, llvm::Type *&Ty)
{
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

std::pair<llvm::Value *, llvm::ShuffleVectorInst *> CostInstructionModel::getShuffleAndOtherOprd(llvm::BinaryOperator *B)
{

	llvm::Value *L = B->getOperand(0);
	llvm::Value *R = B->getOperand(1);
	llvm::ShuffleVectorInst *S = nullptr;

	if ((S = llvm::dyn_cast<llvm::ShuffleVectorInst>(L)))
		return std::make_pair(R, S);

	S = llvm::dyn_cast<llvm::ShuffleVectorInst>(R);
	return std::make_pair(L, S);
}

bool CostInstructionModel::isReverseVectorMask(llvm::ArrayRef<int> Mask)
{
  for (unsigned i = 0, MaskSize = Mask.size(); i < MaskSize; ++i)
    if (Mask[i] >= 0 && Mask[i] != (int)(MaskSize - 1 - i))
      return false;
  return true;
}

bool CostInstructionModel::isSingleSourceVectorMask(llvm::ArrayRef<int> Mask)
{
  bool Vec0 = false;
  bool Vec1 = false;
  for (unsigned i = 0, NumVecElts = Mask.size(); i < NumVecElts; ++i) {
    if (Mask[i] >= 0) {
      if ((unsigned)Mask[i] >= NumVecElts)
        Vec1 = true;
      else
        Vec0 = true;
    }
  }
  return !(Vec0 && Vec1);
}

bool CostInstructionModel::isZeroEltBroadcastVectorMask(llvm::ArrayRef<int> Mask)
{
  for (unsigned i = 0; i < Mask.size(); ++i)
    if (Mask[i] > 0)
      return false;
  return true;
}

bool CostInstructionModel::isAlternateVectorMask(llvm::ArrayRef<int> Mask)
{
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