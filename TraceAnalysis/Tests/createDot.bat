@echo Creating dot files

SET /P inputFile=Enter file LLVM IR file path:
opt -dot-cfg -analyze "%inputFile%"