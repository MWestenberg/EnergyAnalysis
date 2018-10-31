; ModuleID = '/tmp/fac.c'
source_filename = "/tmp/fac.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: nounwind readnone ssp uwtable
define i32 @fac(i32) local_unnamed_addr #0 {
  %2 = icmp eq i32 %0, 0
  br i1 %2, label %79, label %3

; <label>:3:                                      ; preds = %1
  %4 = icmp ult i32 %0, 8
  br i1 %4, label %70, label %5

; <label>:5:                                      ; preds = %3
  %6 = and i32 %0, -8
  %7 = sub i32 %0, %6
  %8 = insertelement <4 x i32> undef, i32 %0, i32 0
  %9 = shufflevector <4 x i32> %8, <4 x i32> undef, <4 x i32> zeroinitializer
  %10 = add <4 x i32> %9, <i32 0, i32 -1, i32 -2, i32 -3>
  %11 = add i32 %6, -8
  %12 = lshr exact i32 %11, 3
  %13 = add nuw nsw i32 %12, 1
  %14 = and i32 %13, 3
  %15 = icmp ult i32 %11, 24
  br i1 %15, label %41, label %16

; <label>:16:                                     ; preds = %5
  %17 = sub nsw i32 %13, %14
  br label %18

; <label>:18:                                     ; preds = %18, %16
  %19 = phi <4 x i32> [ %10, %16 ], [ %38, %18 ]
  %20 = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %16 ], [ %36, %18 ]
  %21 = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %16 ], [ %37, %18 ]
  %22 = phi i32 [ %17, %16 ], [ %39, %18 ]
  %23 = add <4 x i32> %19, <i32 -4, i32 -4, i32 -4, i32 -4>
  %24 = mul nsw <4 x i32> %19, %20
  %25 = mul nsw <4 x i32> %23, %21
  %26 = add <4 x i32> %19, <i32 -8, i32 -8, i32 -8, i32 -8>
  %27 = add <4 x i32> %19, <i32 -12, i32 -12, i32 -12, i32 -12>
  %28 = mul nsw <4 x i32> %26, %24
  %29 = mul nsw <4 x i32> %27, %25
  %30 = add <4 x i32> %19, <i32 -16, i32 -16, i32 -16, i32 -16>
  %31 = add <4 x i32> %19, <i32 -20, i32 -20, i32 -20, i32 -20>
  %32 = mul nsw <4 x i32> %30, %28
  %33 = mul nsw <4 x i32> %31, %29
  %34 = add <4 x i32> %19, <i32 -24, i32 -24, i32 -24, i32 -24>
  %35 = add <4 x i32> %19, <i32 -28, i32 -28, i32 -28, i32 -28>
  %36 = mul nsw <4 x i32> %34, %32
  %37 = mul nsw <4 x i32> %35, %33
  %38 = add <4 x i32> %19, <i32 -32, i32 -32, i32 -32, i32 -32>
  %39 = add i32 %22, -4
  %40 = icmp eq i32 %39, 0
  br i1 %40, label %41, label %18, !llvm.loop !3

; <label>:41:                                     ; preds = %18, %5
  %42 = phi <4 x i32> [ undef, %5 ], [ %36, %18 ]
  %43 = phi <4 x i32> [ undef, %5 ], [ %37, %18 ]
  %44 = phi <4 x i32> [ %10, %5 ], [ %38, %18 ]
  %45 = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %5 ], [ %36, %18 ]
  %46 = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %5 ], [ %37, %18 ]
  %47 = icmp eq i32 %14, 0
  br i1 %47, label %60, label %48

; <label>:48:                                     ; preds = %41
  br label %49

; <label>:49:                                     ; preds = %49, %48
  %50 = phi <4 x i32> [ %44, %48 ], [ %57, %49 ]
  %51 = phi <4 x i32> [ %45, %48 ], [ %55, %49 ]
  %52 = phi <4 x i32> [ %46, %48 ], [ %56, %49 ]
  %53 = phi i32 [ %14, %48 ], [ %58, %49 ]
  %54 = add <4 x i32> %50, <i32 -4, i32 -4, i32 -4, i32 -4>
  %55 = mul nsw <4 x i32> %50, %51
  %56 = mul nsw <4 x i32> %54, %52
  %57 = add <4 x i32> %50, <i32 -8, i32 -8, i32 -8, i32 -8>
  %58 = add i32 %53, -1
  %59 = icmp eq i32 %58, 0
  br i1 %59, label %60, label %49, !llvm.loop !5

; <label>:60:                                     ; preds = %49, %41
  %61 = phi <4 x i32> [ %42, %41 ], [ %55, %49 ]
  %62 = phi <4 x i32> [ %43, %41 ], [ %56, %49 ]
  %63 = mul <4 x i32> %62, %61
  %64 = shufflevector <4 x i32> %63, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  %65 = mul <4 x i32> %63, %64
  %66 = shufflevector <4 x i32> %65, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  %67 = mul <4 x i32> %65, %66
  %68 = extractelement <4 x i32> %67, i32 0
  %69 = icmp eq i32 %6, %0
  br i1 %69, label %79, label %70

; <label>:70:                                     ; preds = %60, %3
  %71 = phi i32 [ %0, %3 ], [ %7, %60 ]
  %72 = phi i32 [ 1, %3 ], [ %68, %60 ]
  br label %73

; <label>:73:                                     ; preds = %70, %73
  %74 = phi i32 [ %76, %73 ], [ %71, %70 ]
  %75 = phi i32 [ %77, %73 ], [ %72, %70 ]
  %76 = add nsw i32 %74, -1
  %77 = mul nsw i32 %74, %75
  %78 = icmp eq i32 %76, 0
  br i1 %78, label %79, label %73, !llvm.loop !7

; <label>:79:                                     ; preds = %73, %60, %1
  %80 = phi i32 [ 1, %1 ], [ %68, %60 ], [ %77, %73 ]
  ret i32 %80
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #1 {
  ret i32 6
}

attributes #0 = { nounwind readnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { norecurse nounwind readnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
!3 = distinct !{!3, !4}
!4 = !{!"llvm.loop.isvectorized", i32 1}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.unroll.disable"}
!7 = distinct !{!7, !8, !4}
!8 = !{!"llvm.loop.unroll.runtime.disable"}
