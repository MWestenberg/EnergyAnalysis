; ModuleID = 'K:\Dev\TraceAnalysis\Tests/LightBulb/lightbulb-trace.ll'
source_filename = "lightbulb-trace.c"
target datalayout = "e-m:w-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.16.27024"

@.str0 = private unnamed_addr constant [9 x i8] c"delay,\000\00"
@.str2 = private unnamed_addr constant [8 x i8] c"loop,\002\00"
@.str5 = private unnamed_addr constant [8 x i8] c"loop,\005\00"
@.str7 = private unnamed_addr constant [8 x i8] c"loop,\007\00"
@.str12 = private unnamed_addr constant [9 x i8] c"loop,\0012\00"
@.str15 = private unnamed_addr constant [9 x i8] c"loop,\0015\00"
@.str2.1 = private unnamed_addr constant [8 x i8] c"main,\002\00"
@.str4 = private unnamed_addr constant [8 x i8] c"main,\004\00"
@.str9 = private unnamed_addr constant [8 x i8] c"main,\009\00"
@.str12.2 = private unnamed_addr constant [9 x i8] c"main,\0012\00"
@.str17 = private unnamed_addr constant [9 x i8] c"main,\0017\00"

; Function Attrs: nounwind uwtable
define dso_local void @delay() local_unnamed_addr #0 {
  %1 = tail call i32 bitcast (i32 (...)* @sleep to i32 (i32)*)(i32 5) #2
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str0, i32 0, i32 0))
  ret void
}

declare dso_local i32 @sleep(...) local_unnamed_addr #1

; Function Attrs: nounwind uwtable
define dso_local void @loop(i32* nocapture readnone, i64) local_unnamed_addr #0 {
  %3 = icmp eq i64 %1, 0
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str2, i32 0, i32 0))
  br i1 %3, label %5, label %7

; <label>:5:                                      ; preds = %15, %2
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str5, i32 0, i32 0))
  ret void

; <label>:7:                                      ; preds = %15, %2
  %8 = phi i64 [ %17, %15 ], [ 0, %2 ]
  %9 = and i64 %8, 1
  %10 = icmp eq i64 %9, 0
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str7, i32 0, i32 0))
  br i1 %10, label %12, label %15

; <label>:12:                                     ; preds = %7
  tail call void bitcast (void (...)* @turnOn to void ()*)() #2
  %13 = tail call i32 bitcast (i32 (...)* @sleep to i32 (i32)*)(i32 5) #2
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str12, i32 0, i32 0))
  br label %15

; <label>:15:                                     ; preds = %12, %7
  tail call void bitcast (void (...)* @turnOff to void ()*)() #2
  %16 = tail call i32 bitcast (i32 (...)* @sleep to i32 (i32)*)(i32 5) #2
  %17 = add nuw i64 %8, 1
  %18 = icmp eq i64 %17, %1
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str15, i32 0, i32 0))
  br i1 %18, label %5, label %7
}

declare dso_local void @turnOn(...) local_unnamed_addr #1

declare dso_local void @turnOff(...) local_unnamed_addr #1

; Function Attrs: nounwind uwtable
define dso_local i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #0 {
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str2.1, i32 0, i32 0))
  br label %4

; <label>:4:                                      ; preds = %12, %2
  %5 = phi i64 [ %14, %12 ], [ 0, %2 ]
  %6 = and i64 %5, 1
  %7 = icmp eq i64 %6, 0
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str4, i32 0, i32 0))
  br i1 %7, label %9, label %12

; <label>:9:                                      ; preds = %4
  tail call void bitcast (void (...)* @turnOn to void ()*)() #2
  %10 = tail call i32 bitcast (i32 (...)* @sleep to i32 (i32)*)(i32 5) #2
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str9, i32 0, i32 0))
  br label %12

; <label>:12:                                     ; preds = %9, %4
  tail call void bitcast (void (...)* @turnOff to void ()*)() #2
  %13 = tail call i32 bitcast (i32 (...)* @sleep to i32 (i32)*)(i32 5) #2
  %14 = add nuw nsw i64 %5, 1
  %15 = icmp eq i64 %14, 17
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str12.2, i32 0, i32 0))
  br i1 %15, label %17, label %4

; <label>:17:                                     ; preds = %12
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str17, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.linker.options = !{!0, !0}
!llvm.module.flags = !{!1, !2}
!llvm.ident = !{!3}

!0 = !{!"/DEFAULTLIB:uuid.lib"}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{i32 7, !"PIC Level", i32 2}
!3 = !{!"clang version 7.0.0 (tags/RELEASE_700/final)"}