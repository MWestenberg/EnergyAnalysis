; ModuleID = 'simpleLoop-alt.c'
source_filename = "simpleLoop-alt.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @radioOn() local_unnamed_addr #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @radioOff() local_unnamed_addr #0 {
  ret void
}

; Function Attrs: nounwind ssp uwtable
define i32 @main() local_unnamed_addr #1 {
  tail call void @__EnergyUsage(i32 5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 0, i32 5, i32 1) #4
  %1 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 0)
  tail call void @__EnergyUsage(i32 -5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 0, i32 5, i32 1) #4
  %2 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 1)
  tail call void @__EnergyUsage(i32 -5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 0, i32 5, i32 1) #4
  %3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 2)
  tail call void @__EnergyUsage(i32 -5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 0, i32 5, i32 1) #4
  %4 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 3)
  tail call void @__EnergyUsage(i32 -5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 0, i32 5, i32 1) #4
  %5 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 4)
  tail call void @__EnergyUsage(i32 -5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 0, i32 5, i32 1) #4
  %6 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 5)
  tail call void @__EnergyUsage(i32 -5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 0, i32 5, i32 1) #4
  %7 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 6)
  tail call void @__EnergyUsage(i32 -5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 0, i32 5, i32 1) #4
  %8 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 7)
  tail call void @__EnergyUsage(i32 -5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 0, i32 5, i32 1) #4
  %9 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 8)
  tail call void @__EnergyUsage(i32 -5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 5, i32 0, i32 0) #4
  tail call void @__EnergyUsage(i32 0, i32 5, i32 1) #4
  %10 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 9)
  tail call void @__EnergyUsage(i32 -5, i32 0, i32 0) #4
  ret i32 1
}

declare void @__EnergyUsage(i32, i32, i32) local_unnamed_addr #2

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #3

attributes #0 = { norecurse nounwind readnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
