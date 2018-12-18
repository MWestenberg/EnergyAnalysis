; ModuleID = 'lightbulb-trace.c'
source_filename = "lightbulb-trace.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: nounwind ssp uwtable
define void @delay() local_unnamed_addr #0 {
  %1 = tail call i32 @"\01_sleep"(i32 5) #2
  ret void
}

declare i32 @"\01_sleep"(i32) local_unnamed_addr #1

; Function Attrs: nounwind ssp uwtable
define void @loop(i32* nocapture readnone, i64) local_unnamed_addr #0 {
  %3 = icmp eq i64 %1, 0
  br i1 %3, label %5, label %4

; <label>:4:                                      ; preds = %2
  br label %6

; <label>:5:                                      ; preds = %11, %2
  ret void

; <label>:6:                                      ; preds = %4, %11
  %7 = phi i64 [ %12, %11 ], [ 0, %4 ]
  %8 = and i64 %7, 1
  %9 = icmp eq i64 %8, 0
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %6
  tail call void (...) @turnOn() #2
  tail call void @delay()
  br label %11

; <label>:11:                                     ; preds = %10, %6
  tail call void (...) @turnOff() #2
  tail call void @delay()
  %12 = add nuw i64 %7, 1
  %13 = icmp eq i64 %12, %1
  br i1 %13, label %5, label %6
}

declare void @turnOn(...) local_unnamed_addr #1

declare void @turnOff(...) local_unnamed_addr #1

; Function Attrs: nounwind ssp uwtable
define i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #0 {
  tail call void @loop(i32* undef, i64 17)
  ret i32 0
}

attributes #0 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
