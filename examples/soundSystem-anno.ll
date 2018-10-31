; ModuleID = 'soundSystem-anno.c'
source_filename = "soundSystem-anno.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: nounwind ssp uwtable
define i32 @main() local_unnamed_addr #0 {
  tail call void (...) @soundSystemOn() #2
  tail call void @playBeep(float 2.000000e+01) #2
  tail call void @sleepABit(i32 5) #2
  tail call void @playBeep(float 2.000000e+01) #2
  tail call void @sleepABit(i32 5) #2
  tail call void @playBeep(float 2.000000e+01) #2
  tail call void @sleepABit(i32 5) #2
  tail call void @playBeep(float 2.000000e+01) #2
  tail call void @sleepABit(i32 5) #2
  tail call void @playBeep(float 2.000000e+01) #2
  tail call void @sleepABit(i32 5) #2
  tail call void (...) @soundSystemOff() #2
  ret i32 1
}

declare void @soundSystemOn(...) local_unnamed_addr #1

declare void @playBeep(float) local_unnamed_addr #1

declare void @sleepABit(i32) local_unnamed_addr #1

declare void @soundSystemOff(...) local_unnamed_addr #1

attributes #0 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
