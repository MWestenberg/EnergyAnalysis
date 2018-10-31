; ModuleID = 'loopbranch.c'
source_filename = "loopbranch.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

@str = private unnamed_addr constant [3 x i8] c"m1\00"
@str.2 = private unnamed_addr constant [3 x i8] c"m2\00"

; Function Attrs: nounwind ssp uwtable
define void @method1() local_unnamed_addr #0 {
  %1 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i64 0, i64 0))
  ret void
}

; Function Attrs: nounwind ssp uwtable
define void @method2() local_unnamed_addr #0 {
  %1 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i64 0, i64 0))
  ret void
}

; Function Attrs: nounwind ssp uwtable
define void @loop(i32* nocapture readonly, i64) local_unnamed_addr #0 {
  %3 = icmp eq i64 %1, 0
  br i1 %3, label %29, label %4

; <label>:4:                                      ; preds = %2
  %5 = add i64 %1, -1
  %6 = and i64 %1, 7
  %7 = icmp ult i64 %5, 7
  br i1 %7, label %10, label %8

; <label>:8:                                      ; preds = %4
  %9 = sub i64 %1, %6
  br label %30

; <label>:10:                                     ; preds = %111, %4
  %11 = phi i64 [ 0, %4 ], [ %112, %111 ]
  %12 = icmp eq i64 %6, 0
  br i1 %12, label %29, label %13

; <label>:13:                                     ; preds = %10
  br label %14

; <label>:14:                                     ; preds = %13, %25
  %15 = phi i64 [ %26, %25 ], [ %11, %13 ]
  %16 = phi i64 [ %27, %25 ], [ %6, %13 ]
  %17 = getelementptr inbounds i32, i32* %0, i64 %15
  %18 = load i32, i32* %17, align 4, !tbaa !3
  %19 = and i32 %18, 1
  %20 = icmp eq i32 %19, 0
  br i1 %20, label %23, label %21

; <label>:21:                                     ; preds = %14
  %22 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i64 0, i64 0)) #1
  br label %25

; <label>:23:                                     ; preds = %14
  %24 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i64 0, i64 0)) #1
  br label %25

; <label>:25:                                     ; preds = %23, %21
  %26 = add nuw i64 %15, 1
  %27 = add i64 %16, -1
  %28 = icmp eq i64 %27, 0
  br i1 %28, label %29, label %14, !llvm.loop !7

; <label>:29:                                     ; preds = %25, %10, %2
  ret void

; <label>:30:                                     ; preds = %111, %8
  %31 = phi i64 [ 0, %8 ], [ %112, %111 ]
  %32 = phi i64 [ %9, %8 ], [ %113, %111 ]
  %33 = getelementptr inbounds i32, i32* %0, i64 %31
  %34 = load i32, i32* %33, align 4, !tbaa !3
  %35 = and i32 %34, 1
  %36 = icmp eq i32 %35, 0
  br i1 %36, label %37, label %39

; <label>:37:                                     ; preds = %30
  %38 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i64 0, i64 0)) #1
  br label %41

; <label>:39:                                     ; preds = %30
  %40 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i64 0, i64 0)) #1
  br label %41

; <label>:41:                                     ; preds = %37, %39
  %42 = or i64 %31, 1
  %43 = getelementptr inbounds i32, i32* %0, i64 %42
  %44 = load i32, i32* %43, align 4, !tbaa !3
  %45 = and i32 %44, 1
  %46 = icmp eq i32 %45, 0
  br i1 %46, label %49, label %47

; <label>:47:                                     ; preds = %41
  %48 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i64 0, i64 0)) #1
  br label %51

; <label>:49:                                     ; preds = %41
  %50 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i64 0, i64 0)) #1
  br label %51

; <label>:51:                                     ; preds = %49, %47
  %52 = or i64 %31, 2
  %53 = getelementptr inbounds i32, i32* %0, i64 %52
  %54 = load i32, i32* %53, align 4, !tbaa !3
  %55 = and i32 %54, 1
  %56 = icmp eq i32 %55, 0
  br i1 %56, label %59, label %57

; <label>:57:                                     ; preds = %51
  %58 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i64 0, i64 0)) #1
  br label %61

; <label>:59:                                     ; preds = %51
  %60 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i64 0, i64 0)) #1
  br label %61

; <label>:61:                                     ; preds = %59, %57
  %62 = or i64 %31, 3
  %63 = getelementptr inbounds i32, i32* %0, i64 %62
  %64 = load i32, i32* %63, align 4, !tbaa !3
  %65 = and i32 %64, 1
  %66 = icmp eq i32 %65, 0
  br i1 %66, label %69, label %67

; <label>:67:                                     ; preds = %61
  %68 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i64 0, i64 0)) #1
  br label %71

; <label>:69:                                     ; preds = %61
  %70 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i64 0, i64 0)) #1
  br label %71

; <label>:71:                                     ; preds = %69, %67
  %72 = or i64 %31, 4
  %73 = getelementptr inbounds i32, i32* %0, i64 %72
  %74 = load i32, i32* %73, align 4, !tbaa !3
  %75 = and i32 %74, 1
  %76 = icmp eq i32 %75, 0
  br i1 %76, label %79, label %77

; <label>:77:                                     ; preds = %71
  %78 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i64 0, i64 0)) #1
  br label %81

; <label>:79:                                     ; preds = %71
  %80 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i64 0, i64 0)) #1
  br label %81

; <label>:81:                                     ; preds = %79, %77
  %82 = or i64 %31, 5
  %83 = getelementptr inbounds i32, i32* %0, i64 %82
  %84 = load i32, i32* %83, align 4, !tbaa !3
  %85 = and i32 %84, 1
  %86 = icmp eq i32 %85, 0
  br i1 %86, label %89, label %87

; <label>:87:                                     ; preds = %81
  %88 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i64 0, i64 0)) #1
  br label %91

; <label>:89:                                     ; preds = %81
  %90 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i64 0, i64 0)) #1
  br label %91

; <label>:91:                                     ; preds = %89, %87
  %92 = or i64 %31, 6
  %93 = getelementptr inbounds i32, i32* %0, i64 %92
  %94 = load i32, i32* %93, align 4, !tbaa !3
  %95 = and i32 %94, 1
  %96 = icmp eq i32 %95, 0
  br i1 %96, label %99, label %97

; <label>:97:                                     ; preds = %91
  %98 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i64 0, i64 0)) #1
  br label %101

; <label>:99:                                     ; preds = %91
  %100 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i64 0, i64 0)) #1
  br label %101

; <label>:101:                                    ; preds = %99, %97
  %102 = or i64 %31, 7
  %103 = getelementptr inbounds i32, i32* %0, i64 %102
  %104 = load i32, i32* %103, align 4, !tbaa !3
  %105 = and i32 %104, 1
  %106 = icmp eq i32 %105, 0
  br i1 %106, label %109, label %107

; <label>:107:                                    ; preds = %101
  %108 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i64 0, i64 0)) #1
  br label %111

; <label>:109:                                    ; preds = %101
  %110 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i64 0, i64 0)) #1
  br label %111

; <label>:111:                                    ; preds = %109, %107
  %112 = add i64 %31, 8
  %113 = add i64 %32, -8
  %114 = icmp eq i64 %113, 0
  br i1 %114, label %10, label %30, !llvm.loop !9
}

; Function Attrs: nounwind ssp uwtable
define i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #0 {
  br label %3

; <label>:3:                                      ; preds = %12, %2
  %4 = phi i64 [ 0, %2 ], [ %13, %12 ]
  %5 = phi i64 [ 4, %2 ], [ %14, %12 ]
  %6 = or i64 %4, 2
  %7 = icmp eq i64 %6, 3
  br i1 %7, label %10, label %8

; <label>:8:                                      ; preds = %3
  %9 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i64 0, i64 0)) #1
  br label %12

; <label>:10:                                     ; preds = %3
  %11 = tail call i32 @puts(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i64 0, i64 0)) #1
  br label %12

; <label>:12:                                     ; preds = %10, %8
  %13 = add nuw nsw i64 %4, 1
  %14 = add nsw i64 %5, -1
  %15 = icmp eq i64 %14, 0
  br i1 %15, label %16, label %3, !llvm.loop !7

; <label>:16:                                     ; preds = %12
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) local_unnamed_addr #1

attributes #0 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = distinct !{!7, !8}
!8 = !{!"llvm.loop.unroll.disable"}
!9 = distinct !{!9, !8}
