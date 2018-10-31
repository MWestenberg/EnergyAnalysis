; ModuleID = 'plateCutter.c'
source_filename = "plateCutter.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: noinline nounwind ssp uwtable
define void @resetController() local_unnamed_addr #0 {
  %1 = tail call zeroext i1 (...) @start_assembly_line() #2
  br i1 %1, label %2, label %16

; <label>:2:                                      ; preds = %0
  %3 = tail call zeroext i1 (...) @start_sensor() #2
  br i1 %3, label %4, label %16

; <label>:4:                                      ; preds = %2
  %5 = tail call zeroext i1 (...) @sensor_read() #2
  %6 = tail call zeroext i1 (...) @stop_sensor() #2
  %7 = and i1 %5, %6
  br i1 %7, label %8, label %16

; <label>:8:                                      ; preds = %4
  %9 = tail call <2 x float> (...) @measure_plate() #2
  %10 = tail call zeroext i1 @fire_laser(<2 x float> %9) #2
  br label %11

; <label>:11:                                     ; preds = %11, %8
  %12 = tail call zeroext i1 (...) @laser_firing() #2
  %13 = tail call i32 @"\01_sleep"(i32 5) #2
  br i1 %12, label %11, label %14

; <label>:14:                                     ; preds = %11
  %15 = tail call zeroext i1 (...) @stop_laser() #2
  br label %16

; <label>:16:                                     ; preds = %4, %2, %14, %0
  ret void
}

declare zeroext i1 @start_assembly_line(...) local_unnamed_addr #1

declare zeroext i1 @start_sensor(...) local_unnamed_addr #1

declare zeroext i1 @sensor_read(...) local_unnamed_addr #1

declare zeroext i1 @stop_sensor(...) local_unnamed_addr #1

declare <2 x float> @measure_plate(...) local_unnamed_addr #1

declare zeroext i1 @fire_laser(<2 x float>) local_unnamed_addr #1

declare zeroext i1 @laser_firing(...) local_unnamed_addr #1

declare i32 @"\01_sleep"(i32) local_unnamed_addr #1

declare zeroext i1 @stop_laser(...) local_unnamed_addr #1

; Function Attrs: noinline nounwind ssp uwtable
define i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #0 {
  tail call void @resetController()
  ret i32 0
}

attributes #0 = { noinline nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
