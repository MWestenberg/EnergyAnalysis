; ModuleID = 'soundSystem1.c'
source_filename = "soundSystem1.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

@.str = private unnamed_addr constant [8 x i8] c"15,20,0\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [15 x i8] c"soundSystem1.c\00", section "llvm.metadata"
@llvm.global.annotations = appending global [4 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @soundSystemOn to i8*), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str.1, i32 0, i32 0), i32 18 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @soundSystemOff to i8*), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str.1, i32 0, i32 0), i32 19 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void (float)* @playBeep to i8*), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str.1, i32 0, i32 0), i32 20 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void (i32)* @sleepABit to i8*), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str.1, i32 0, i32 0), i32 21 }], section "llvm.metadata"

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @soundSystemOn() #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @soundSystemOff() #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @playBeep(float) #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @sleepABit(i32) #0 {
  ret void
}

; Function Attrs: nounwind ssp uwtable
define i32 @main() local_unnamed_addr #1 {
  tail call void @__EnergyUsage(i32 5, i32 20, i32 0) #3
  tail call void @__EnergyUsage(i32 0, i32 5, i32 2) #3
  tail call void @__EnergyUsage(i32 0, i32 0, i32 5) #3
  tail call void @__EnergyUsage(i32 0, i32 5, i32 2) #3
  tail call void @__EnergyUsage(i32 0, i32 0, i32 5) #3
  tail call void @__EnergyUsage(i32 0, i32 5, i32 2) #3
  tail call void @__EnergyUsage(i32 0, i32 0, i32 5) #3
  tail call void @__EnergyUsage(i32 0, i32 5, i32 2) #3
  tail call void @__EnergyUsage(i32 0, i32 0, i32 5) #3
  tail call void @__EnergyUsage(i32 0, i32 5, i32 2) #3
  tail call void @__EnergyUsage(i32 0, i32 0, i32 5) #3
  tail call void @__EnergyUsage(i32 -5, i32 20, i32 0) #3
  ret i32 1
}

declare void @__EnergyUsage(i32, i32, i32) local_unnamed_addr #2

attributes #0 = { norecurse nounwind readnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
