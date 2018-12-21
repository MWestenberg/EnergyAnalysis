; ModuleID = 'LightBulb\lightbulb-anno.c'
source_filename = "LightBulb\5Clightbulb-anno.c"
target datalayout = "e-m:w-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.16.27025"

@.str = private unnamed_addr constant [13 x i8] c"Light,40,0,0\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [27 x i8] c"LightBulb\5Clightbulb-anno.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [12 x i8] c"Light,0,0,0\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [12 x i8] c"delay,0,0,5\00", section "llvm.metadata"
@llvm.global.annotations = appending global [3 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_turnOn to i8*), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.1, i32 0, i32 0), i32 18 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_turnOff to i8*), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.1, i32 0, i32 0), i32 19 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_delay to i8*), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.1, i32 0, i32 0), i32 21 }], section "llvm.metadata"

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local void @_turnOn() #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local void @_turnOff() #0 {
  ret void
}

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local void @_delay() #0 {
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @loop(i32* nocapture readnone, i64) local_unnamed_addr #1 {
  %3 = icmp eq i64 %1, 0
  br i1 %3, label %4, label %5

; <label>:4:                                      ; preds = %10, %2
  ret void

; <label>:5:                                      ; preds = %2, %10
  %6 = phi i64 [ %11, %10 ], [ 0, %2 ]
  %7 = and i64 %6, 1
  %8 = icmp eq i64 %7, 0
  br i1 %8, label %9, label %10

; <label>:9:                                      ; preds = %5
  tail call void bitcast (void (...)* @turnOn to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  br label %10

; <label>:10:                                     ; preds = %9, %5
  tail call void bitcast (void (...)* @turnOff to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  %11 = add nuw i64 %6, 1
  %12 = icmp eq i64 %11, %1
  br i1 %12, label %4, label %5
}

declare dso_local void @turnOn(...) local_unnamed_addr #2

declare dso_local void @delay(...) local_unnamed_addr #2

declare dso_local void @turnOff(...) local_unnamed_addr #2

; Function Attrs: nounwind uwtable
define dso_local i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #1 {
  br label %3

; <label>:3:                                      ; preds = %8, %2
  %4 = phi i64 [ %9, %8 ], [ 0, %2 ]
  %5 = and i64 %4, 1
  %6 = icmp eq i64 %5, 0
  br i1 %6, label %7, label %8

; <label>:7:                                      ; preds = %3
  tail call void bitcast (void (...)* @turnOn to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  br label %8

; <label>:8:                                      ; preds = %7, %3
  tail call void bitcast (void (...)* @turnOff to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  %9 = add nuw nsw i64 %4, 1
  %10 = icmp eq i64 %9, 17
  br i1 %10, label %11, label %3

; <label>:11:                                     ; preds = %8
  ret i32 0
}

attributes #0 = { norecurse nounwind readnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.linker.options = !{!0, !0}
!llvm.module.flags = !{!1, !2}
!llvm.ident = !{!3}

!0 = !{!"/DEFAULTLIB:uuid.lib"}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{i32 7, !"PIC Level", i32 2}
!3 = !{!"clang version 7.0.0 (tags/RELEASE_700/final)"}
