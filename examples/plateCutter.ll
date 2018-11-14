; ModuleID = 'platecutter.c'
source_filename = "platecutter.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

@.str = private unnamed_addr constant [21 x i8] c"AssemblyLine,0,100,2\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [14 x i8] c"platecutter.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [18 x i8] c"PlateSensor,5,5,1\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [22 x i8] c"PlateSensorRead,0,5,2\00", section "llvm.metadata"
@.str.4 = private unnamed_addr constant [18 x i8] c"PlateSensor,0,5,1\00", section "llvm.metadata"
@.str.5 = private unnamed_addr constant [20 x i8] c"Laser,1000,2000,180\00", section "llvm.metadata"
@.str.6 = private unnamed_addr constant [18 x i8] c"LaserFiring,0,0,1\00", section "llvm.metadata"
@.str.7 = private unnamed_addr constant [16 x i8] c"Laser,0,1000,10\00", section "llvm.metadata"
@.str.8 = private unnamed_addr constant [12 x i8] c"Delay,0,0,5\00", section "llvm.metadata"
@.str.9 = private unnamed_addr constant [22 x i8] c"MeasurePlate, 0, 5, 2\00", section "llvm.metadata"
@llvm.global.annotations = appending global [9 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_start_assembly_line to i8*), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.1, i32 0, i32 0), i32 32 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_start_sensor to i8*), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.1, i32 0, i32 0), i32 33 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_sensor_read to i8*), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.1, i32 0, i32 0), i32 34 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_stop_sensor to i8*), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.4, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.1, i32 0, i32 0), i32 35 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_fire_laser to i8*), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.1, i32 0, i32 0), i32 37 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_laser_firing to i8*), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.6, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.1, i32 0, i32 0), i32 38 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_stop_laser to i8*), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.7, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.1, i32 0, i32 0), i32 39 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_delay to i8*), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.8, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.1, i32 0, i32 0), i32 40 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_measure_plate to i8*), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.1, i32 0, i32 0), i32 41 }], section "llvm.metadata"

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @_start_assembly_line() #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @_start_sensor() #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @_sensor_read() #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @_stop_sensor() #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @_fire_laser() #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @_laser_firing() #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @_stop_laser() #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @_delay() #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define void @_measure_plate() #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define zeroext i1 @checkMeasurements(<2 x float>, <2 x float>) local_unnamed_addr #0 {
  ret i1 true
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
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

; Function Attrs: nounwind ssp uwtable
define void @startController() local_unnamed_addr #1 {
  %1 = tail call zeroext i1 (...) @start_assembly_line() #3
  br i1 %1, label %2, label %24

; <label>:2:                                      ; preds = %0
  %3 = tail call zeroext i1 (...) @start_sensor() #3
  br i1 %3, label %4, label %24

; <label>:4:                                      ; preds = %2
  %5 = tail call zeroext i1 (...) @sensor_read() #3
  %6 = tail call zeroext i1 (...) @stop_sensor() #3
  %7 = and i1 %5, %6
  br i1 %7, label %8, label %24

; <label>:8:                                      ; preds = %4
  %9 = tail call { <2 x float>, <2 x float> } (...) @measure_plate() #3
  %10 = extractvalue { <2 x float>, <2 x float> } %9, 1
  %11 = extractvalue { <2 x float>, <2 x float> } %9, 0
  %12 = extractelement <2 x float> %11, i32 0
  %13 = extractelement <2 x float> %10, i32 0
  %14 = fadd float %12, %13
  %15 = fmul float %14, 5.000000e-01
  %16 = insertelement <2 x float> undef, float %15, i32 0
  %17 = shufflevector <2 x float> %16, <2 x float> %10, <2 x i32> <i32 0, i32 3>
  %18 = shufflevector <2 x float> %16, <2 x float> %11, <2 x i32> <i32 0, i32 3>
  %19 = tail call zeroext i1 (<2 x float>, <2 x float>, ...) @fire_laser(<2 x float> %17, <2 x float> %18) #3
  br label %20

; <label>:20:                                     ; preds = %20, %8
  tail call void @LOOP_TRIPCOUNT(i32 5) #3
  tail call void (i32, ...) bitcast (void (...)* @delay to void (i32, ...)*)(i32 5) #3
  %21 = tail call zeroext i1 (...) @laser_firing() #3
  br i1 %21, label %20, label %22

; <label>:22:                                     ; preds = %20
  %23 = tail call zeroext i1 (...) @stop_laser() #3
  br label %24

; <label>:24:                                     ; preds = %4, %2, %22, %0
  ret void
}

declare zeroext i1 @start_assembly_line(...) local_unnamed_addr #2

declare zeroext i1 @start_sensor(...) local_unnamed_addr #2

declare zeroext i1 @sensor_read(...) local_unnamed_addr #2

declare zeroext i1 @stop_sensor(...) local_unnamed_addr #2

declare { <2 x float>, <2 x float> } @measure_plate(...) local_unnamed_addr #2

declare zeroext i1 @fire_laser(<2 x float>, <2 x float>, ...) local_unnamed_addr #2

declare void @LOOP_TRIPCOUNT(i32) local_unnamed_addr #2

declare void @delay(...) local_unnamed_addr #2

declare zeroext i1 @laser_firing(...) local_unnamed_addr #2

declare zeroext i1 @stop_laser(...) local_unnamed_addr #2

; Function Attrs: nounwind ssp uwtable
define i32 @main(i32, i8** nocapture readonly) local_unnamed_addr #1 {
  %3 = icmp slt i32 %0, 1
  br i1 %3, label %37, label %4

; <label>:4:                                      ; preds = %2
  %5 = getelementptr inbounds i8*, i8** %1, i64 1
  %6 = bitcast i8** %5 to i64*
  %7 = load i64, i64* %6, align 8, !tbaa !3
  %8 = trunc i64 %7 to i32
  br label %9

; <label>:9:                                      ; preds = %34, %4
  %10 = phi i32 [ %8, %4 ], [ %35, %34 ]
  tail call void @LOOP_TRIPCOUNT(i32 10) #3
  %11 = tail call zeroext i1 (...) @start_assembly_line() #3
  br i1 %11, label %12, label %34

; <label>:12:                                     ; preds = %9
  %13 = tail call zeroext i1 (...) @start_sensor() #3
  br i1 %13, label %14, label %34

; <label>:14:                                     ; preds = %12
  %15 = tail call zeroext i1 (...) @sensor_read() #3
  %16 = tail call zeroext i1 (...) @stop_sensor() #3
  %17 = and i1 %15, %16
  br i1 %17, label %18, label %34

; <label>:18:                                     ; preds = %14
  %19 = tail call { <2 x float>, <2 x float> } (...) @measure_plate() #3
  %20 = extractvalue { <2 x float>, <2 x float> } %19, 1
  %21 = extractvalue { <2 x float>, <2 x float> } %19, 0
  %22 = extractelement <2 x float> %21, i32 0
  %23 = extractelement <2 x float> %20, i32 0
  %24 = fadd float %22, %23
  %25 = fmul float %24, 5.000000e-01
  %26 = insertelement <2 x float> undef, float %25, i32 0
  %27 = shufflevector <2 x float> %26, <2 x float> %20, <2 x i32> <i32 0, i32 3>
  %28 = shufflevector <2 x float> %26, <2 x float> %21, <2 x i32> <i32 0, i32 3>
  %29 = tail call zeroext i1 (<2 x float>, <2 x float>, ...) @fire_laser(<2 x float> %27, <2 x float> %28) #3
  br label %30

; <label>:30:                                     ; preds = %30, %18
  tail call void @LOOP_TRIPCOUNT(i32 5) #3
  tail call void (i32, ...) bitcast (void (...)* @delay to void (i32, ...)*)(i32 5) #3
  %31 = tail call zeroext i1 (...) @laser_firing() #3
  br i1 %31, label %30, label %32

; <label>:32:                                     ; preds = %30
  %33 = tail call zeroext i1 (...) @stop_laser() #3
  br label %34

; <label>:34:                                     ; preds = %9, %12, %14, %32
  %35 = add nsw i32 %10, -1
  tail call void (i32, ...) bitcast (void (...)* @delay to void (i32, ...)*)(i32 5) #3
  %36 = icmp sgt i32 %10, 1
  br i1 %36, label %9, label %37

; <label>:37:                                     ; preds = %34, %2
  ret i32 0
}

attributes #0 = { norecurse nounwind readnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"any pointer", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
