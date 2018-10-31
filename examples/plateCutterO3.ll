; ModuleID = 'plateCutter.c'
source_filename = "plateCutter.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: noinline norecurse nounwind readnone ssp uwtable
define { <2 x float>, <2 x float> } @CalculateLine(<2 x float>, <2 x float>) local_unnamed_addr #0 {
  %3 = extractelement <2 x float> %0, i32 0
  %4 = extractelement <2 x float> %1, i32 0
  %5 = fadd float %3, %4
  %6 = fmul float %5, 5.000000e-01
  %7 = insertelement <2 x float> undef, float %6, i32 0
  %8 = shufflevector <2 x float> %7, <2 x float> %1, <2 x i32> <i32 0, i32 3>
  %9 = shufflevector <2 x float> %7, <2 x float> %0, <2 x i32> <i32 0, i32 3>
  %10 = insertvalue { <2 x float>, <2 x float> } undef, <2 x float> %8, 0
  %11 = insertvalue { <2 x float>, <2 x float> } %10, <2 x float> %9, 1
  ret { <2 x float>, <2 x float> } %11
}

; Function Attrs: noinline nounwind ssp uwtable
define void @startController() local_unnamed_addr #1 {
  %1 = tail call zeroext i1 (...) @start_assembly_line() #3
  br i1 %1, label %2, label %20

; <label>:2:                                      ; preds = %0
  %3 = tail call zeroext i1 (...) @start_sensor() #3
  br i1 %3, label %4, label %20

; <label>:4:                                      ; preds = %2
  %5 = tail call zeroext i1 (...) @sensor_read() #3
  %6 = tail call zeroext i1 (...) @stop_sensor() #3
  %7 = and i1 %5, %6
  br i1 %7, label %8, label %20

; <label>:8:                                      ; preds = %4
  %9 = tail call { <2 x float>, <2 x float> } (...) @measure_plate() #3
  %10 = extractvalue { <2 x float>, <2 x float> } %9, 0
  %11 = extractvalue { <2 x float>, <2 x float> } %9, 1
  %12 = tail call { <2 x float>, <2 x float> } @CalculateLine(<2 x float> %10, <2 x float> %11)
  %13 = extractvalue { <2 x float>, <2 x float> } %12, 0
  %14 = extractvalue { <2 x float>, <2 x float> } %12, 1
  %15 = tail call zeroext i1 @fire_laser(<2 x float> %13, <2 x float> %14) #3
  br label %16

; <label>:16:                                     ; preds = %16, %8
  %17 = tail call zeroext i1 (...) @laser_firing() #3
  tail call void @sleep(i32 5) #3
  br i1 %17, label %16, label %18

; <label>:18:                                     ; preds = %16
  %19 = tail call zeroext i1 (...) @stop_laser() #3
  br label %20

; <label>:20:                                     ; preds = %4, %2, %18, %0
  ret void
}

declare zeroext i1 @start_assembly_line(...) local_unnamed_addr #2

declare zeroext i1 @start_sensor(...) local_unnamed_addr #2

declare zeroext i1 @sensor_read(...) local_unnamed_addr #2

declare zeroext i1 @stop_sensor(...) local_unnamed_addr #2

declare { <2 x float>, <2 x float> } @measure_plate(...) local_unnamed_addr #2

declare zeroext i1 @fire_laser(<2 x float>, <2 x float>) local_unnamed_addr #2

declare zeroext i1 @laser_firing(...) local_unnamed_addr #2

declare void @sleep(i32) local_unnamed_addr #2

declare zeroext i1 @stop_laser(...) local_unnamed_addr #2

; Function Attrs: noinline nounwind ssp uwtable
define i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #1 {
  tail call void @startController()
  ret i32 0
}

attributes #0 = { noinline norecurse nounwind readnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
