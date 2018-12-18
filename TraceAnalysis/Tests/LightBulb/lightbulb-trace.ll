; ModuleID = 'K:\Dev\TraceAnalysis\Tests/LightBulb/lightbulb-anno.ll'
source_filename = "lightbulb-anno.c"
target datalayout = "e-m:w-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.16.27025"

@.str = private unnamed_addr constant [13 x i8] c"Light,40,0,0\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [17 x i8] c"lightbulb-anno.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [12 x i8] c"Light,0,0,0\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [12 x i8] c"delay,0,0,5\00", section "llvm.metadata"
@llvm.global.annotations = appending global [3 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* undef, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1, i32 0, i32 0), i32 18 }, { i8*, i8*, i8*, i32 } { i8* undef, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1, i32 0, i32 0), i32 19 }, { i8*, i8*, i8*, i32 } { i8* undef, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1, i32 0, i32 0), i32 21 }], section "llvm.metadata"
@.str2 = private unnamed_addr constant [15 x i8] c"F: loop, BB:2\0A\00"
@.str4 = private unnamed_addr constant [15 x i8] c"F: loop, BB:4\0A\00"
@.str5 = private unnamed_addr constant [15 x i8] c"F: loop, BB:5\0A\00"
@.str9 = private unnamed_addr constant [15 x i8] c"F: loop, BB:9\0A\00"
@.str10 = private unnamed_addr constant [16 x i8] c"F: loop, BB:10\0A\00"
@.str2.1 = private unnamed_addr constant [15 x i8] c"F: main, BB:2\0A\00"
@.str3 = private unnamed_addr constant [15 x i8] c"F: main, BB:3\0A\00"
@.str7 = private unnamed_addr constant [15 x i8] c"F: main, BB:7\0A\00"
@.str8 = private unnamed_addr constant [15 x i8] c"F: main, BB:8\0A\00"
@.str11 = private unnamed_addr constant [16 x i8] c"F: main, BB:11\0A\00"

; Function Attrs: nounwind uwtable
define dso_local void @loop(i32* nocapture readnone, i64) local_unnamed_addr #0 {
  %3 = icmp eq i64 %1, 0
  %"2" = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str2, i32 0, i32 0))
  br i1 %3, label %4, label %5

; <label>:4:                                      ; preds = %10, %2
  %"4" = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str4, i32 0, i32 0))
  ret void

; <label>:5:                                      ; preds = %10, %2
  %6 = phi i64 [ %11, %10 ], [ 0, %2 ]
  %7 = and i64 %6, 1
  %8 = icmp eq i64 %7, 0
  %"5" = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str5, i32 0, i32 0))
  br i1 %8, label %9, label %10

; <label>:9:                                      ; preds = %5
  %"9" = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str9, i32 0, i32 0))
  br label %10

; <label>:10:                                     ; preds = %9, %5
  %11 = add nuw i64 %6, 1
  %12 = icmp eq i64 %11, %1
  %"10" = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str10, i32 0, i32 0))
  br i1 %12, label %4, label %5
}

declare dso_local void @turnOn(...) local_unnamed_addr #1

declare dso_local void @delay(...) local_unnamed_addr #2

declare dso_local void @turnOff(...) local_unnamed_addr #3

; Function Attrs: nounwind uwtable
define dso_local i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #0 {
  %"2" = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str2.1, i32 0, i32 0))
  br label %3

; <label>:3:                                      ; preds = %8, %2
  %4 = phi i64 [ %9, %8 ], [ 0, %2 ]
  %5 = and i64 %4, 1
  %6 = icmp eq i64 %5, 0
  %"3" = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str3, i32 0, i32 0))
  br i1 %6, label %7, label %8

; <label>:7:                                      ; preds = %3
  %"7" = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str7, i32 0, i32 0))
  br label %8

; <label>:8:                                      ; preds = %7, %3
  %9 = add nuw nsw i64 %4, 1
  %10 = icmp eq i64 %9, 17
  %"8" = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str8, i32 0, i32 0))
  br i1 %10, label %11, label %3

; <label>:11:                                     ; preds = %8
  %"11" = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str11, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "Energy" "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "ec"="0" "less-precise-fpmad"="false" "name"="Light" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "pd"="40" "stack-protector-buffer-size"="8" "t"="0" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "Energy" "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "ec"="0" "less-precise-fpmad"="false" "name"="delay" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "pd"="0" "stack-protector-buffer-size"="8" "t"="5" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "Energy" "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "ec"="0" "less-precise-fpmad"="false" "name"="Light" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "pd"="0" "stack-protector-buffer-size"="8" "t"="0" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.linker.options = !{!0, !0}
!llvm.module.flags = !{!1, !2}
!llvm.ident = !{!3}

!0 = !{!"/DEFAULTLIB:uuid.lib"}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{i32 7, !"PIC Level", i32 2}
!3 = !{!"clang version 7.0.0 (tags/RELEASE_700/final)"}