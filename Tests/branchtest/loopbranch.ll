; ModuleID = 'branchtest\loopbranch.c'
source_filename = "branchtest\5Cloopbranch.c"
target datalayout = "e-m:w-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.16.27025"

%struct._iobuf = type { i8* }
%struct.__crt_locale_pointers = type { %struct.__crt_locale_data*, %struct.__crt_multibyte_data* }
%struct.__crt_locale_data = type opaque
%struct.__crt_multibyte_data = type opaque

$printf = comdat any

$__local_stdio_printf_options = comdat any

$"??_C@_07JJNKOEKP@m1?3?5?$CFd?6?$AA@" = comdat any

$"??_C@_08HEKEIMC@m2?3?5?$CFd?5?6?$AA@" = comdat any

$"??_C@_07DPEEJGEG@branch1?$AA@" = comdat any

$"??_C@_07BEGJMFIF@branch2?$AA@" = comdat any

$"??_C@_09CEHHDHKE@No?5branch?$AA@" = comdat any

@.str = private unnamed_addr constant [21 x i8] c"AssemblyLine,0,100,2\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [24 x i8] c"branchtest\5Cloopbranch.c\00", section "llvm.metadata"
@"??_C@_07JJNKOEKP@m1?3?5?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"m1: %d\0A\00", comdat, align 1
@"??_C@_08HEKEIMC@m2?3?5?$CFd?5?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [9 x i8] c"m2: %d \0A\00", comdat, align 1
@"??_C@_07DPEEJGEG@branch1?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"branch1\00", comdat, align 1
@"??_C@_07BEGJMFIF@branch2?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"branch2\00", comdat, align 1
@"??_C@_09CEHHDHKE@No?5branch?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"No branch\00", comdat, align 1
@__local_stdio_printf_options._OptionsStorage = internal global i64 0, align 8
@llvm.global.annotations = appending global [1 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (i1 ()* @_start_assembly_line to i8*), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.1, i32 0, i32 0), i32 6 }], section "llvm.metadata"

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local zeroext i1 @_start_assembly_line() #0 {
  ret i1 true
}

; Function Attrs: nounwind uwtable
define dso_local zeroext i1 @method1(i32) local_unnamed_addr #1 {
  %2 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07JJNKOEKP@m1?3?5?$CFd?6?$AA@", i64 0, i64 0), i32 %0)
  %3 = icmp sgt i32 %0, 3
  ret i1 %3
}

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr dso_local i32 @printf(i8*, ...) local_unnamed_addr #2 comdat {
  %2 = alloca i8*, align 8
  %3 = bitcast i8** %2 to i8*
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %3) #5
  call void @llvm.va_start(i8* nonnull %3)
  %4 = load i8*, i8** %2, align 8, !tbaa !4
  %5 = call %struct._iobuf* @__acrt_iob_func(i32 1) #5
  %6 = call i64* @__local_stdio_printf_options() #5
  %7 = load i64, i64* %6, align 8, !tbaa !8
  %8 = call i32 @__stdio_common_vfprintf(i64 %7, %struct._iobuf* %5, i8* %0, %struct.__crt_locale_pointers* null, i8* %4) #5
  call void @llvm.va_end(i8* nonnull %3)
  call void @llvm.lifetime.end.p0i8(i64 8, i8* nonnull %3) #5
  ret i32 %8
}

; Function Attrs: nounwind uwtable
define dso_local zeroext i1 @method2(i32) local_unnamed_addr #1 {
  %2 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"??_C@_08HEKEIMC@m2?3?5?$CFd?5?6?$AA@", i64 0, i64 0), i32 %0)
  %3 = icmp sgt i32 %0, 1
  ret i1 %3
}

; Function Attrs: nounwind uwtable
define dso_local void @loop(i32* nocapture readonly, i64) local_unnamed_addr #1 {
  %3 = tail call zeroext i1 bitcast (i1 (...)* @start_assembly_line to i1 ()*)() #5
  %4 = icmp eq i64 %1, 0
  br i1 %4, label %5, label %6

; <label>:5:                                      ; preds = %25, %2
  ret void

; <label>:6:                                      ; preds = %2, %25
  %7 = phi i64 [ %26, %25 ], [ 0, %2 ]
  %8 = getelementptr inbounds i32, i32* %0, i64 %7
  %9 = load i32, i32* %8, align 4, !tbaa !10
  %10 = and i32 %9, 1
  %11 = icmp eq i32 %10, 0
  br i1 %11, label %12, label %25

; <label>:12:                                     ; preds = %6
  %13 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07JJNKOEKP@m1?3?5?$CFd?6?$AA@", i64 0, i64 0), i32 %9) #5
  %14 = icmp sgt i32 %9, 3
  br i1 %14, label %15, label %17

; <label>:15:                                     ; preds = %12
  %16 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07DPEEJGEG@branch1?$AA@", i64 0, i64 0))
  br label %25

; <label>:17:                                     ; preds = %12
  %18 = load i32, i32* %8, align 4, !tbaa !10
  %19 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"??_C@_08HEKEIMC@m2?3?5?$CFd?5?6?$AA@", i64 0, i64 0), i32 %18) #5
  %20 = icmp sgt i32 %18, 1
  br i1 %20, label %21, label %23

; <label>:21:                                     ; preds = %17
  %22 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07BEGJMFIF@branch2?$AA@", i64 0, i64 0))
  br label %25

; <label>:23:                                     ; preds = %17
  %24 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"??_C@_09CEHHDHKE@No?5branch?$AA@", i64 0, i64 0))
  br label %25

; <label>:25:                                     ; preds = %6, %21, %23, %15
  %26 = add nuw i64 %7, 1
  %27 = icmp eq i64 %26, %1
  br i1 %27, label %5, label %6
}

declare dso_local zeroext i1 @start_assembly_line(...) local_unnamed_addr #3

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #4

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #4

; Function Attrs: nounwind uwtable
define dso_local i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #1 {
  %3 = tail call zeroext i1 bitcast (i1 (...)* @start_assembly_line to i1 ()*)() #5
  %4 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07JJNKOEKP@m1?3?5?$CFd?6?$AA@", i64 0, i64 0), i32 2) #5
  %5 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"??_C@_08HEKEIMC@m2?3?5?$CFd?5?6?$AA@", i64 0, i64 0), i32 2) #5
  %6 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07BEGJMFIF@branch2?$AA@", i64 0, i64 0)) #5
  %7 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07JJNKOEKP@m1?3?5?$CFd?6?$AA@", i64 0, i64 0), i32 4) #5
  %8 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07DPEEJGEG@branch1?$AA@", i64 0, i64 0)) #5
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.va_start(i8*) #5

; Function Attrs: nounwind
declare void @llvm.va_end(i8*) #5

; Function Attrs: noinline norecurse nounwind uwtable
define linkonce_odr dso_local i64* @__local_stdio_printf_options() local_unnamed_addr #6 comdat {
  ret i64* @__local_stdio_printf_options._OptionsStorage
}

declare dso_local %struct._iobuf* @__acrt_iob_func(i32) local_unnamed_addr #3

declare dso_local i32 @__stdio_common_vfprintf(i64, %struct._iobuf*, i8*, %struct.__crt_locale_pointers*, i8*) local_unnamed_addr #3

attributes #0 = { norecurse nounwind readnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { inlinehint nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { argmemonly nounwind }
attributes #5 = { nounwind }
attributes #6 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.linker.options = !{!0, !0}
!llvm.module.flags = !{!1, !2}
!llvm.ident = !{!3}

!0 = !{!"/DEFAULTLIB:uuid.lib"}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{i32 7, !"PIC Level", i32 2}
!3 = !{!"clang version 7.0.0 (tags/RELEASE_700/final)"}
!4 = !{!5, !5, i64 0}
!5 = !{!"any pointer", !6, i64 0}
!6 = !{!"omnipotent char", !7, i64 0}
!7 = !{!"Simple C/C++ TBAA"}
!8 = !{!9, !9, i64 0}
!9 = !{!"long long", !6, i64 0}
!10 = !{!11, !11, i64 0}
!11 = !{!"int", !6, i64 0}
