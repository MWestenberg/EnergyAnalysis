#include "WCETAnalysisPass.h"


char WCETAnalysisPass::ID = 1;


void WCETAnalysisPass::getAnalysisUsage(llvm::AnalysisUsage & AU) const
{
	AU.addRequired<llvm::TargetTransformInfoWrapperPass>();
	AU.addRequired<llvm::LoopInfoWrapperPass>();
	AU.addRequired<llvm::ScalarEvolutionWrapperPass>();
	AU.setPreservesAll();
}

bool WCETAnalysisPass::runOnFunction(llvm::Function & F)
{
	this->F = &F;
	

	auto *TTIWP = getAnalysisIfAvailable<llvm::TargetTransformInfoWrapperPass>();
	TTI = TTIWP ? &TTIWP->getTTI(F) : nullptr;


	return true;
}

unsigned WCETAnalysisPass::getInstructionCost(const llvm::Instruction * I) const
{
	if (!TTI)
		return -1;

	switch (I->getOpcode()) {
	case llvm::Instruction::GetElementPtr: {
		llvm::Type *ValTy = I->getOperand(0)->getType()->getPointerElementType();
		return TTI->getAddressComputationCost(ValTy);
	}

	case llvm::Instruction::Ret:
	case llvm::Instruction::PHI:
	case llvm::Instruction::Br: {
		return TTI->getCFInstrCost(I->getOpcode());
	}
	case llvm::Instruction::Add:
	case llvm::Instruction::FAdd:
		return 2;
	case llvm::Instruction::Sub:
	case llvm::Instruction::FSub:
		return 2;
	case llvm::Instruction::Mul:
	case llvm::Instruction::FMul:
		return 2;
	case llvm::Instruction::UDiv:
	case llvm::Instruction::SDiv:
	case llvm::Instruction::FDiv:
		return 2;
	case llvm::Instruction::URem:
	case llvm::Instruction::SRem:
	case llvm::Instruction::FRem:
	case llvm::Instruction::Shl:
	case llvm::Instruction::LShr:
	case llvm::Instruction::AShr:
	case llvm::Instruction::And:
	case llvm::Instruction::Or:
	case llvm::Instruction::Xor: {
		llvm::TargetTransformInfo::OperandValueKind Op1VK =
			getOperandInfo(I->getOperand(0));
		llvm::TargetTransformInfo::OperandValueKind Op2VK =
			getOperandInfo(I->getOperand(1));
		return TTI->getArithmeticInstrCost(I->getOpcode(), I->getType(), Op1VK,
			Op2VK);
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
		else if (matchPairwiseReduction(EEI, ReduxOpCode, ReduxType))
			return TTI->getArithmeticReductionCost(ReduxOpCode, ReduxType, true);

		return TTI->getVectorInstrCost(I->getOpcode(),
			EEI->getOperand(0)->getType(), Idx);
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

void WCETAnalysisPass::print(llvm::raw_ostream &OS, const llvm::Module* Module) const {
	if (!F)
		return;

	for (llvm::BasicBlock &B : *F) {
		for (llvm::Instruction &Inst : B) {
			unsigned Cost = getInstructionCost(&Inst);
			if (Cost != (unsigned)-1)
				OS << "Cost Model: Found an estimated cost of " << Cost;
			else
				OS << "Cost Model: Unknown cost";

			OS << " for instruction: " << Inst << "\n";
		}
	}
}
