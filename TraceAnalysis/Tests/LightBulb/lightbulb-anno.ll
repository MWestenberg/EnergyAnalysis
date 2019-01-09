; ModuleID = 'lightbulb-anno.c'
source_filename = "lightbulb-anno.c"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i686-pc-windows-msvc19.16.27025"

@.str = private unnamed_addr constant [13 x i8] c"Light,25,0,0\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [17 x i8] c"lightbulb-anno.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [12 x i8] c"Light,0,0,0\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [12 x i8] c"delay,0,0,2\00", section "llvm.metadata"
@.str.4 = private unnamed_addr constant [12 x i8] c"delay,0,0,8\00", section "llvm.metadata"
@done = dso_local local_unnamed_addr global i8 0, align 1
@d = dso_local local_unnamed_addr global [6 x i32] [i32 5, i32 3, i32 10, i32 38, i32 22, i32 40], align 4
@llvm.global.annotations = appending global [4 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_turnOn to i8*), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1, i32 0, i32 0), i32 18 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_turnOff to i8*), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1, i32 0, i32 0), i32 19 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_delayOff to i8*), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1, i32 0, i32 0), i32 21 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_delayOn to i8*), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.4, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1, i32 0, i32 0), i32 22 }], section "llvm.metadata"

; Function Attrs: norecurse nounwind readnone
define dso_local void @_turnOn() #0 {
entry:
  ret void
}

; Function Attrs: norecurse nounwind readnone
define dso_local void @_turnOff() #0 {
entry:
  ret void
}

; Function Attrs: norecurse nounwind readnone
define dso_local void @_delayOff() #0 {
entry:
  ret void
}

; Function Attrs: norecurse nounwind readnone
define dso_local void @_delayOn() #0 {
entry:
  ret void
}

; Function Attrs: nounwind
define dso_local void @loop() local_unnamed_addr #1 {
entry:
  %0 = load i8, i8* @done, align 1, !tbaa !3, !range !7
  %tobool = icmp eq i8 %0, 0
  br i1 %tobool, label %for.cond1.preheader.preheader, label %return

for.cond1.preheader.preheader:                    ; preds = %entry
  %1 = load i32, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @d, i32 0, i32 0), align 4, !tbaa !8
  %cmp221 = icmp sgt i32 %1, 0
  br i1 %cmp221, label %for.body4, label %for.cond.cleanup3

for.cond.cleanup3:                                ; preds = %if.end8, %for.cond1.preheader.preheader
  %2 = load i32, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @d, i32 0, i32 1), align 4, !tbaa !8
  %cmp221.1 = icmp sgt i32 %2, 0
  br i1 %cmp221.1, label %for.body4.1, label %for.cond.cleanup3.1

for.body4:                                        ; preds = %for.cond1.preheader.preheader, %if.end8
  %i.022 = phi i32 [ %inc, %if.end8 ], [ 0, %for.cond1.preheader.preheader ]
  %rem = urem i32 %i.022, 5
  %rem.off = add nsw i32 %rem, -1
  %3 = icmp ult i32 %rem.off, 3
  br i1 %3, label %if.then7, label %if.else

if.then7:                                         ; preds = %for.body4
  tail call void bitcast (void (...)* @turnOff to void ()*)() #3
  tail call void bitcast (void (...)* @delayOff to void ()*)() #3
  br label %if.end8

if.else:                                          ; preds = %for.body4
  tail call void bitcast (void (...)* @turnOn to void ()*)() #3
  tail call void bitcast (void (...)* @delayOn to void ()*)() #3
  br label %if.end8

if.end8:                                          ; preds = %if.else, %if.then7
  %inc = add nuw nsw i32 %i.022, 1
  %4 = load i32, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @d, i32 0, i32 0), align 4, !tbaa !8
  %cmp2 = icmp slt i32 %inc, %4
  br i1 %cmp2, label %for.body4, label %for.cond.cleanup3

return:                                           ; preds = %entry, %for.cond.cleanup3.5
  ret void

for.body4.1:                                      ; preds = %for.cond.cleanup3, %if.end8.1
  %i.022.1 = phi i32 [ %inc.1, %if.end8.1 ], [ 0, %for.cond.cleanup3 ]
  %rem.1 = urem i32 %i.022.1, 5
  %rem.off.1 = add nsw i32 %rem.1, -1
  %5 = icmp ult i32 %rem.off.1, 3
  br i1 %5, label %if.then7.1, label %if.else.1

if.else.1:                                        ; preds = %for.body4.1
  tail call void bitcast (void (...)* @turnOn to void ()*)() #3
  tail call void bitcast (void (...)* @delayOn to void ()*)() #3
  br label %if.end8.1

if.then7.1:                                       ; preds = %for.body4.1
  tail call void bitcast (void (...)* @turnOff to void ()*)() #3
  tail call void bitcast (void (...)* @delayOff to void ()*)() #3
  br label %if.end8.1

if.end8.1:                                        ; preds = %if.then7.1, %if.else.1
  %inc.1 = add nuw nsw i32 %i.022.1, 1
  %6 = load i32, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @d, i32 0, i32 1), align 4, !tbaa !8
  %cmp2.1 = icmp slt i32 %inc.1, %6
  br i1 %cmp2.1, label %for.body4.1, label %for.cond.cleanup3.1

for.cond.cleanup3.1:                              ; preds = %if.end8.1, %for.cond.cleanup3
  %7 = load i32, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @d, i32 0, i32 2), align 4, !tbaa !8
  %cmp221.2 = icmp sgt i32 %7, 0
  br i1 %cmp221.2, label %for.body4.2, label %for.cond.cleanup3.2

for.body4.2:                                      ; preds = %for.cond.cleanup3.1, %if.end8.2
  %i.022.2 = phi i32 [ %inc.2, %if.end8.2 ], [ 0, %for.cond.cleanup3.1 ]
  %rem.2 = urem i32 %i.022.2, 5
  %rem.off.2 = add nsw i32 %rem.2, -1
  %8 = icmp ult i32 %rem.off.2, 3
  br i1 %8, label %if.then7.2, label %if.else.2

if.else.2:                                        ; preds = %for.body4.2
  tail call void bitcast (void (...)* @turnOn to void ()*)() #3
  tail call void bitcast (void (...)* @delayOn to void ()*)() #3
  br label %if.end8.2

if.then7.2:                                       ; preds = %for.body4.2
  tail call void bitcast (void (...)* @turnOff to void ()*)() #3
  tail call void bitcast (void (...)* @delayOff to void ()*)() #3
  br label %if.end8.2

if.end8.2:                                        ; preds = %if.then7.2, %if.else.2
  %inc.2 = add nuw nsw i32 %i.022.2, 1
  %9 = load i32, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @d, i32 0, i32 2), align 4, !tbaa !8
  %cmp2.2 = icmp slt i32 %inc.2, %9
  br i1 %cmp2.2, label %for.body4.2, label %for.cond.cleanup3.2

for.cond.cleanup3.2:                              ; preds = %if.end8.2, %for.cond.cleanup3.1
  %10 = load i32, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @d, i32 0, i32 3), align 4, !tbaa !8
  %cmp221.3 = icmp sgt i32 %10, 0
  br i1 %cmp221.3, label %for.body4.3, label %for.cond.cleanup3.3

for.body4.3:                                      ; preds = %for.cond.cleanup3.2, %if.end8.3
  %i.022.3 = phi i32 [ %inc.3, %if.end8.3 ], [ 0, %for.cond.cleanup3.2 ]
  %rem.3 = urem i32 %i.022.3, 5
  %rem.off.3 = add nsw i32 %rem.3, -1
  %11 = icmp ult i32 %rem.off.3, 3
  br i1 %11, label %if.then7.3, label %if.else.3

if.else.3:                                        ; preds = %for.body4.3
  tail call void bitcast (void (...)* @turnOn to void ()*)() #3
  tail call void bitcast (void (...)* @delayOn to void ()*)() #3
  br label %if.end8.3

if.then7.3:                                       ; preds = %for.body4.3
  tail call void bitcast (void (...)* @turnOff to void ()*)() #3
  tail call void bitcast (void (...)* @delayOff to void ()*)() #3
  br label %if.end8.3

if.end8.3:                                        ; preds = %if.then7.3, %if.else.3
  %inc.3 = add nuw nsw i32 %i.022.3, 1
  %12 = load i32, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @d, i32 0, i32 3), align 4, !tbaa !8
  %cmp2.3 = icmp slt i32 %inc.3, %12
  br i1 %cmp2.3, label %for.body4.3, label %for.cond.cleanup3.3

for.cond.cleanup3.3:                              ; preds = %if.end8.3, %for.cond.cleanup3.2
  %13 = load i32, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @d, i32 0, i32 4), align 4, !tbaa !8
  %cmp221.4 = icmp sgt i32 %13, 0
  br i1 %cmp221.4, label %for.body4.4, label %for.cond.cleanup3.4

for.body4.4:                                      ; preds = %for.cond.cleanup3.3, %if.end8.4
  %i.022.4 = phi i32 [ %inc.4, %if.end8.4 ], [ 0, %for.cond.cleanup3.3 ]
  %rem.4 = urem i32 %i.022.4, 5
  %rem.off.4 = add nsw i32 %rem.4, -1
  %14 = icmp ult i32 %rem.off.4, 3
  br i1 %14, label %if.then7.4, label %if.else.4

if.else.4:                                        ; preds = %for.body4.4
  tail call void bitcast (void (...)* @turnOn to void ()*)() #3
  tail call void bitcast (void (...)* @delayOn to void ()*)() #3
  br label %if.end8.4

if.then7.4:                                       ; preds = %for.body4.4
  tail call void bitcast (void (...)* @turnOff to void ()*)() #3
  tail call void bitcast (void (...)* @delayOff to void ()*)() #3
  br label %if.end8.4

if.end8.4:                                        ; preds = %if.then7.4, %if.else.4
  %inc.4 = add nuw nsw i32 %i.022.4, 1
  %15 = load i32, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @d, i32 0, i32 4), align 4, !tbaa !8
  %cmp2.4 = icmp slt i32 %inc.4, %15
  br i1 %cmp2.4, label %for.body4.4, label %for.cond.cleanup3.4

for.cond.cleanup3.4:                              ; preds = %if.end8.4, %for.cond.cleanup3.3
  %16 = load i32, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @d, i32 0, i32 5), align 4, !tbaa !8
  %cmp221.5 = icmp sgt i32 %16, 0
  br i1 %cmp221.5, label %for.body4.5, label %for.cond.cleanup3.5

for.body4.5:                                      ; preds = %for.cond.cleanup3.4, %if.end8.5
  %i.022.5 = phi i32 [ %inc.5, %if.end8.5 ], [ 0, %for.cond.cleanup3.4 ]
  %rem.5 = urem i32 %i.022.5, 5
  %rem.off.5 = add nsw i32 %rem.5, -1
  %17 = icmp ult i32 %rem.off.5, 3
  br i1 %17, label %if.then7.5, label %if.else.5

if.else.5:                                        ; preds = %for.body4.5
  tail call void bitcast (void (...)* @turnOn to void ()*)() #3
  tail call void bitcast (void (...)* @delayOn to void ()*)() #3
  br label %if.end8.5

if.then7.5:                                       ; preds = %for.body4.5
  tail call void bitcast (void (...)* @turnOff to void ()*)() #3
  tail call void bitcast (void (...)* @delayOff to void ()*)() #3
  br label %if.end8.5

if.end8.5:                                        ; preds = %if.then7.5, %if.else.5
  %inc.5 = add nuw nsw i32 %i.022.5, 1
  %18 = load i32, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @d, i32 0, i32 5), align 4, !tbaa !8
  %cmp2.5 = icmp slt i32 %inc.5, %18
  br i1 %cmp2.5, label %for.body4.5, label %for.cond.cleanup3.5

for.cond.cleanup3.5:                              ; preds = %if.end8.5, %for.cond.cleanup3.4
  tail call void bitcast (void (...)* @turnOff to void ()*)() #3
  store i8 1, i8* @done, align 1, !tbaa !3
  br label %return
}

declare dso_local void @turnOff(...) local_unnamed_addr #2

declare dso_local void @delayOff(...) local_unnamed_addr #2

declare dso_local void @turnOn(...) local_unnamed_addr #2

declare dso_local void @delayOn(...) local_unnamed_addr #2

; Function Attrs: nounwind
define dso_local i32 @main(i32 %argc, i8** nocapture readnone %argv) local_unnamed_addr #1 {
entry:
  tail call void @loop()
  ret i32 0
}

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{!"clang version 8.0.0 (https://github.com/llvm-mirror/clang.git bfbe338a893dde6ba65b2bed6ffea1652a592819) (https://github.com/llvm-mirror/llvm.git a99d6d2122ca2f208e1c4bcaf02ff5930f244f34)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"_Bool", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = !{i8 0, i8 2}
!8 = !{!9, !9, i64 0}
!9 = !{!"int", !5, i64 0}
