; ModuleID = 'Fibonnaci\fibonnaci.c'
source_filename = "Fibonnaci\5Cfibonnaci.c"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i686-pc-windows-msvc19.16.27025"

@.str = private unnamed_addr constant [29 x i8] c"VacuumCleaner,590,0,0.004534\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [22 x i8] c"Fibonnaci\5Cfibonnaci.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [27 x i8] c"VacuumCleaner,0,0,0.004534\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [26 x i8] c"Serial.println, 0,0,0.785\00", section "llvm.metadata"
@.str.4 = private unnamed_addr constant [19 x i8] c"delay20, 0,0,20000\00", section "llvm.metadata"
@.str.5 = private unnamed_addr constant [17 x i8] c"delay8, 0,0,8000\00", section "llvm.metadata"
@relayPin = dso_local local_unnamed_addr constant i32 7, align 4
@d = dso_local local_unnamed_addr global [7 x i32] [i32 5, i32 3, i32 7, i32 10, i32 120, i32 51, i32 38], align 4
@done = dso_local local_unnamed_addr global i8 0, align 1
@llvm.global.annotations = appending global [5 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_digitalWriteHIGH to i8*), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 5 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_digitalWriteLOW to i8*), i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 6 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_SerialPrintln to i8*), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 7 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_delay20 to i8*), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.4, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 8 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_delay8 to i8*), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 9 }], section "llvm.metadata"

; Function Attrs: norecurse nounwind readnone
define dso_local void @_digitalWriteHIGH() #0 {
entry:
  ret void
}

; Function Attrs: norecurse nounwind readnone
define dso_local void @_digitalWriteLOW() #0 {
entry:
  ret void
}

; Function Attrs: norecurse nounwind readnone
define dso_local void @_SerialPrintln() #0 {
entry:
  ret void
}

; Function Attrs: norecurse nounwind readnone
define dso_local void @_delay20() #0 {
entry:
  ret void
}

; Function Attrs: norecurse nounwind readnone
define dso_local void @_delay8() #0 {
entry:
  ret void
}

; Function Attrs: nounwind
define dso_local void @turnOn() local_unnamed_addr #1 {
entry:
  tail call void (i32, ...) @digitalWriteHIGH(i32 7, i32 1) #3
  ret void
}

declare dso_local void @digitalWriteHIGH(i32, ...) local_unnamed_addr #2

; Function Attrs: nounwind
define dso_local void @turnOff() local_unnamed_addr #1 {
entry:
  tail call void (i32, ...) @digitalWriteLOW(i32 7, i32 0) #3
  ret void
}

declare dso_local void @digitalWriteLOW(i32, ...) local_unnamed_addr #2

; Function Attrs: nounwind
define dso_local i32 @fibonacci(i32 %n) local_unnamed_addr #1 {
entry:
  %cmp11 = icmp sgt i32 %n, 0
  br i1 %cmp11, label %for.body, label %for.cond.cleanup

for.cond.for.cond.cleanup_crit_edge:              ; preds = %for.body
  %0 = add i32 %n, -1
  %next.1.le = select i1 %cmp1, i32 %0, i32 %add
  br label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.cond.for.cond.cleanup_crit_edge, %entry
  %next.0.lcssa = phi i32 [ %next.1.le, %for.cond.for.cond.cleanup_crit_edge ], [ undef, %entry ]
  tail call void (i32, ...) @delay20(i32 20000) #3
  ret i32 %next.0.lcssa

for.body:                                         ; preds = %entry, %for.body
  %c.014 = phi i32 [ %inc, %for.body ], [ 0, %entry ]
  %second.013 = phi i32 [ %second.1, %for.body ], [ 1, %entry ]
  %first.012 = phi i32 [ %first.1, %for.body ], [ 0, %entry ]
  %cmp1 = icmp ult i32 %c.014, 2
  %add = add nsw i32 %second.013, %first.012
  %first.1 = select i1 %cmp1, i32 %first.012, i32 %second.013
  %second.1 = select i1 %cmp1, i32 %second.013, i32 %add
  %inc = add nuw nsw i32 %c.014, 1
  %exitcond = icmp eq i32 %inc, %n
  br i1 %exitcond, label %for.cond.for.cond.cleanup_crit_edge, label %for.body
}

declare dso_local void @delay20(i32, ...) local_unnamed_addr #2

; Function Attrs: nounwind
define dso_local void @loop() local_unnamed_addr #1 {
entry:
  %0 = load i8, i8* @done, align 1, !tbaa !3, !range !7
  %tobool = icmp eq i8 %0, 0
  br i1 %tobool, label %for.cond1.preheader.preheader, label %return

for.cond1.preheader.preheader:                    ; preds = %entry
  %1 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 0), align 4, !tbaa !8
  %cmp223 = icmp sgt i32 %1, 0
  br i1 %cmp223, label %for.body4, label %for.cond.cleanup3

for.cond.cleanup3:                                ; preds = %if.end9, %for.cond1.preheader.preheader
  %2 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 1), align 4, !tbaa !8
  %cmp223.1 = icmp sgt i32 %2, 0
  br i1 %cmp223.1, label %for.body4.1, label %for.cond.cleanup3.1

for.body4:                                        ; preds = %for.cond1.preheader.preheader, %if.end9
  %i.024 = phi i32 [ %inc, %if.end9 ], [ 0, %for.cond1.preheader.preheader ]
  %rem = urem i32 %i.024, 5
  %rem.off = add nsw i32 %rem, -1
  %3 = icmp ult i32 %rem.off, 3
  br i1 %3, label %if.then7, label %if.else

if.then7:                                         ; preds = %for.body4
  tail call void (i32, ...) @digitalWriteLOW(i32 7, i32 0) #3
  tail call void (i32, ...) @delay8(i32 4000) #3
  br label %if.end9

if.else:                                          ; preds = %for.body4
  tail call void (i32, ...) @digitalWriteHIGH(i32 7, i32 1) #3
  tail call void (i32, ...) @delay20(i32 20000) #3
  br label %if.end9

if.end9:                                          ; preds = %if.else, %if.then7
  %inc = add nuw nsw i32 %i.024, 1
  %4 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 0), align 4, !tbaa !8
  %cmp2 = icmp slt i32 %inc, %4
  br i1 %cmp2, label %for.body4, label %for.cond.cleanup3

return:                                           ; preds = %entry, %for.cond.cleanup3.6
  ret void

for.body4.1:                                      ; preds = %for.cond.cleanup3, %if.end9.1
  %i.024.1 = phi i32 [ %inc.1, %if.end9.1 ], [ 0, %for.cond.cleanup3 ]
  %rem.1 = urem i32 %i.024.1, 5
  %rem.off.1 = add nsw i32 %rem.1, -1
  %5 = icmp ult i32 %rem.off.1, 3
  br i1 %5, label %if.then7.1, label %if.else.1

if.else.1:                                        ; preds = %for.body4.1
  tail call void (i32, ...) @digitalWriteHIGH(i32 7, i32 1) #3
  tail call void (i32, ...) @delay20(i32 20000) #3
  br label %if.end9.1

if.then7.1:                                       ; preds = %for.body4.1
  tail call void (i32, ...) @digitalWriteLOW(i32 7, i32 0) #3
  tail call void (i32, ...) @delay8(i32 4000) #3
  br label %if.end9.1

if.end9.1:                                        ; preds = %if.then7.1, %if.else.1
  %inc.1 = add nuw nsw i32 %i.024.1, 1
  %6 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 1), align 4, !tbaa !8
  %cmp2.1 = icmp slt i32 %inc.1, %6
  br i1 %cmp2.1, label %for.body4.1, label %for.cond.cleanup3.1

for.cond.cleanup3.1:                              ; preds = %if.end9.1, %for.cond.cleanup3
  %7 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 2), align 4, !tbaa !8
  %cmp223.2 = icmp sgt i32 %7, 0
  br i1 %cmp223.2, label %for.body4.2, label %for.cond.cleanup3.2

for.body4.2:                                      ; preds = %for.cond.cleanup3.1, %if.end9.2
  %i.024.2 = phi i32 [ %inc.2, %if.end9.2 ], [ 0, %for.cond.cleanup3.1 ]
  %rem.2 = urem i32 %i.024.2, 5
  %rem.off.2 = add nsw i32 %rem.2, -1
  %8 = icmp ult i32 %rem.off.2, 3
  br i1 %8, label %if.then7.2, label %if.else.2

if.else.2:                                        ; preds = %for.body4.2
  tail call void (i32, ...) @digitalWriteHIGH(i32 7, i32 1) #3
  tail call void (i32, ...) @delay20(i32 20000) #3
  br label %if.end9.2

if.then7.2:                                       ; preds = %for.body4.2
  tail call void (i32, ...) @digitalWriteLOW(i32 7, i32 0) #3
  tail call void (i32, ...) @delay8(i32 4000) #3
  br label %if.end9.2

if.end9.2:                                        ; preds = %if.then7.2, %if.else.2
  %inc.2 = add nuw nsw i32 %i.024.2, 1
  %9 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 2), align 4, !tbaa !8
  %cmp2.2 = icmp slt i32 %inc.2, %9
  br i1 %cmp2.2, label %for.body4.2, label %for.cond.cleanup3.2

for.cond.cleanup3.2:                              ; preds = %if.end9.2, %for.cond.cleanup3.1
  %10 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 3), align 4, !tbaa !8
  %cmp223.3 = icmp sgt i32 %10, 0
  br i1 %cmp223.3, label %for.body4.3, label %for.cond.cleanup3.3

for.body4.3:                                      ; preds = %for.cond.cleanup3.2, %if.end9.3
  %i.024.3 = phi i32 [ %inc.3, %if.end9.3 ], [ 0, %for.cond.cleanup3.2 ]
  %rem.3 = urem i32 %i.024.3, 5
  %rem.off.3 = add nsw i32 %rem.3, -1
  %11 = icmp ult i32 %rem.off.3, 3
  br i1 %11, label %if.then7.3, label %if.else.3

if.else.3:                                        ; preds = %for.body4.3
  tail call void (i32, ...) @digitalWriteHIGH(i32 7, i32 1) #3
  tail call void (i32, ...) @delay20(i32 20000) #3
  br label %if.end9.3

if.then7.3:                                       ; preds = %for.body4.3
  tail call void (i32, ...) @digitalWriteLOW(i32 7, i32 0) #3
  tail call void (i32, ...) @delay8(i32 4000) #3
  br label %if.end9.3

if.end9.3:                                        ; preds = %if.then7.3, %if.else.3
  %inc.3 = add nuw nsw i32 %i.024.3, 1
  %12 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 3), align 4, !tbaa !8
  %cmp2.3 = icmp slt i32 %inc.3, %12
  br i1 %cmp2.3, label %for.body4.3, label %for.cond.cleanup3.3

for.cond.cleanup3.3:                              ; preds = %if.end9.3, %for.cond.cleanup3.2
  %13 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 4), align 4, !tbaa !8
  %cmp223.4 = icmp sgt i32 %13, 0
  br i1 %cmp223.4, label %for.body4.4, label %for.cond.cleanup3.4

for.body4.4:                                      ; preds = %for.cond.cleanup3.3, %if.end9.4
  %i.024.4 = phi i32 [ %inc.4, %if.end9.4 ], [ 0, %for.cond.cleanup3.3 ]
  %rem.4 = urem i32 %i.024.4, 5
  %rem.off.4 = add nsw i32 %rem.4, -1
  %14 = icmp ult i32 %rem.off.4, 3
  br i1 %14, label %if.then7.4, label %if.else.4

if.else.4:                                        ; preds = %for.body4.4
  tail call void (i32, ...) @digitalWriteHIGH(i32 7, i32 1) #3
  tail call void (i32, ...) @delay20(i32 20000) #3
  br label %if.end9.4

if.then7.4:                                       ; preds = %for.body4.4
  tail call void (i32, ...) @digitalWriteLOW(i32 7, i32 0) #3
  tail call void (i32, ...) @delay8(i32 4000) #3
  br label %if.end9.4

if.end9.4:                                        ; preds = %if.then7.4, %if.else.4
  %inc.4 = add nuw nsw i32 %i.024.4, 1
  %15 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 4), align 4, !tbaa !8
  %cmp2.4 = icmp slt i32 %inc.4, %15
  br i1 %cmp2.4, label %for.body4.4, label %for.cond.cleanup3.4

for.cond.cleanup3.4:                              ; preds = %if.end9.4, %for.cond.cleanup3.3
  %16 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 5), align 4, !tbaa !8
  %cmp223.5 = icmp sgt i32 %16, 0
  br i1 %cmp223.5, label %for.body4.5, label %for.cond.cleanup3.5

for.body4.5:                                      ; preds = %for.cond.cleanup3.4, %if.end9.5
  %i.024.5 = phi i32 [ %inc.5, %if.end9.5 ], [ 0, %for.cond.cleanup3.4 ]
  %rem.5 = urem i32 %i.024.5, 5
  %rem.off.5 = add nsw i32 %rem.5, -1
  %17 = icmp ult i32 %rem.off.5, 3
  br i1 %17, label %if.then7.5, label %if.else.5

if.else.5:                                        ; preds = %for.body4.5
  tail call void (i32, ...) @digitalWriteHIGH(i32 7, i32 1) #3
  tail call void (i32, ...) @delay20(i32 20000) #3
  br label %if.end9.5

if.then7.5:                                       ; preds = %for.body4.5
  tail call void (i32, ...) @digitalWriteLOW(i32 7, i32 0) #3
  tail call void (i32, ...) @delay8(i32 4000) #3
  br label %if.end9.5

if.end9.5:                                        ; preds = %if.then7.5, %if.else.5
  %inc.5 = add nuw nsw i32 %i.024.5, 1
  %18 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 5), align 4, !tbaa !8
  %cmp2.5 = icmp slt i32 %inc.5, %18
  br i1 %cmp2.5, label %for.body4.5, label %for.cond.cleanup3.5

for.cond.cleanup3.5:                              ; preds = %if.end9.5, %for.cond.cleanup3.4
  %19 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 6), align 4, !tbaa !8
  %cmp223.6 = icmp sgt i32 %19, 0
  br i1 %cmp223.6, label %for.body4.6, label %for.cond.cleanup3.6

for.body4.6:                                      ; preds = %for.cond.cleanup3.5, %if.end9.6
  %i.024.6 = phi i32 [ %inc.6, %if.end9.6 ], [ 0, %for.cond.cleanup3.5 ]
  %rem.6 = urem i32 %i.024.6, 5
  %rem.off.6 = add nsw i32 %rem.6, -1
  %20 = icmp ult i32 %rem.off.6, 3
  br i1 %20, label %if.then7.6, label %if.else.6

if.else.6:                                        ; preds = %for.body4.6
  tail call void (i32, ...) @digitalWriteHIGH(i32 7, i32 1) #3
  tail call void (i32, ...) @delay20(i32 20000) #3
  br label %if.end9.6

if.then7.6:                                       ; preds = %for.body4.6
  tail call void (i32, ...) @digitalWriteLOW(i32 7, i32 0) #3
  tail call void (i32, ...) @delay8(i32 4000) #3
  br label %if.end9.6

if.end9.6:                                        ; preds = %if.then7.6, %if.else.6
  %inc.6 = add nuw nsw i32 %i.024.6, 1
  %21 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @d, i32 0, i32 6), align 4, !tbaa !8
  %cmp2.6 = icmp slt i32 %inc.6, %21
  br i1 %cmp2.6, label %for.body4.6, label %for.cond.cleanup3.6

for.cond.cleanup3.6:                              ; preds = %if.end9.6, %for.cond.cleanup3.5
  tail call void (i32, ...) @digitalWriteLOW(i32 7, i32 0) #3
  store i8 1, i8* @done, align 1, !tbaa !3
  br label %return
}

declare dso_local void @delay8(i32, ...) local_unnamed_addr #2

; Function Attrs: nounwind
define dso_local i32 @main() local_unnamed_addr #1 {
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
