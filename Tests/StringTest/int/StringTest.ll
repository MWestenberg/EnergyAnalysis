; ModuleID = 'StringTest\StringTest.c'
source_filename = "StringTest\5CStringTest.c"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i686-pc-windows-msvc19.16.27025"

@.str = private unnamed_addr constant [24 x i8] c"LightBulb,71,0,0.004534\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [24 x i8] c"StringTest\5CStringTest.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [23 x i8] c"LightBulb,0,0,0.004534\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [26 x i8] c"Serial.println, 0,0,0.500\00", section "llvm.metadata"
@.str.4 = private unnamed_addr constant [19 x i8] c"delayOff, 0,0,2000\00", section "llvm.metadata"
@.str.5 = private unnamed_addr constant [18 x i8] c"delayOn, 0,0,3000\00", section "llvm.metadata"
@relayPin = dso_local local_unnamed_addr constant i32 7, align 4
@llvm.global.annotations = appending global [5 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_digitalWriteHIGH to i8*), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.1, i32 0, i32 0), i32 5 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_digitalWriteLOW to i8*), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.1, i32 0, i32 0), i32 6 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_SerialPrintln to i8*), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.1, i32 0, i32 0), i32 7 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_delayOff to i8*), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.4, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.1, i32 0, i32 0), i32 8 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_delayOn to i8*), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.1, i32 0, i32 0), i32 9 }], section "llvm.metadata"

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
define dso_local void @turnOff() local_unnamed_addr #1 {
entry:
  tail call void (i32, ...) @digitalWriteLOW(i32 7, i32 0) #3
  tail call void (i32, ...) @delayOff(i32 2000) #3
  ret void
}

declare dso_local void @digitalWriteLOW(i32, ...) local_unnamed_addr #2

declare dso_local void @delayOff(i32, ...) local_unnamed_addr #2

; Function Attrs: nounwind
define dso_local void @turnOn() local_unnamed_addr #1 {
entry:
  tail call void (i32, ...) @digitalWriteHIGH(i32 7, i32 1) #3
  tail call void (i32, ...) @delayOn(i32 3000) #3
  ret void
}

declare dso_local void @digitalWriteHIGH(i32, ...) local_unnamed_addr #2

declare dso_local void @delayOn(i32, ...) local_unnamed_addr #2

; Function Attrs: nounwind
define dso_local void @loop(i8* nocapture readonly %input, i32 %n) local_unnamed_addr #1 {
entry:
  %cmp6 = icmp sgt i32 %n, 0
  br i1 %cmp6, label %for.body, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.inc, %entry
  ret void

for.body:                                         ; preds = %entry, %for.inc
  %i.07 = phi i32 [ %inc, %for.inc ], [ 0, %entry ]
  %arrayidx = getelementptr inbounds i8, i8* %input, i32 %i.07
  %0 = load i8, i8* %arrayidx, align 1, !tbaa !3
  %cmp1 = icmp eq i8 %0, 97
  br i1 %cmp1, label %if.then, label %if.else

if.then:                                          ; preds = %for.body
  tail call void (i32, ...) @digitalWriteHIGH(i32 7, i32 1) #3
  tail call void (i32, ...) @delayOn(i32 3000) #3
  br label %for.inc

if.else:                                          ; preds = %for.body
  tail call void (i32, ...) @digitalWriteLOW(i32 7, i32 0) #3
  tail call void (i32, ...) @delayOff(i32 2000) #3
  br label %for.inc

for.inc:                                          ; preds = %if.then, %if.else
  %inc = add nuw nsw i32 %i.07, 1
  %exitcond = icmp eq i32 %inc, %n
  br i1 %exitcond, label %for.cond.cleanup, label %for.body
}

; Function Attrs: nounwind
define dso_local i32 @main(i32 %argc, i8** nocapture readonly %argv) local_unnamed_addr #1 {
entry:
  %cmp = icmp sgt i32 %argc, 1
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %add.ptr = getelementptr inbounds i8*, i8** %argv, i32 1
  %0 = load i8*, i8** %add.ptr, align 4, !tbaa !6
  %1 = load i8, i8* %0, align 1, !tbaa !3
  %cmp19 = icmp eq i8 %1, 0
  br i1 %cmp19, label %if.end, label %for.body

for.body.i:                                       ; preds = %for.body, %for.inc.i.for.body.i_crit_edge
  %2 = phi i8 [ %.pre, %for.inc.i.for.body.i_crit_edge ], [ %1, %for.body ]
  %i.07.i = phi i32 [ %inc.i, %for.inc.i.for.body.i_crit_edge ], [ 0, %for.body ]
  %cmp1.i = icmp eq i8 %2, 97
  br i1 %cmp1.i, label %if.then.i, label %if.else.i

if.then.i:                                        ; preds = %for.body.i
  tail call void (i32, ...) @digitalWriteHIGH(i32 7, i32 1) #3
  tail call void (i32, ...) @delayOn(i32 3000) #3
  br label %for.inc.i

if.else.i:                                        ; preds = %for.body.i
  tail call void (i32, ...) @digitalWriteLOW(i32 7, i32 0) #3
  tail call void (i32, ...) @delayOff(i32 2000) #3
  br label %for.inc.i

for.inc.i:                                        ; preds = %if.else.i, %if.then.i
  %inc.i = add nuw nsw i32 %i.07.i, 1
  %exitcond.i = icmp eq i32 %inc.i, %inc
  br i1 %exitcond.i, label %if.end, label %for.inc.i.for.body.i_crit_edge

for.inc.i.for.body.i_crit_edge:                   ; preds = %for.inc.i
  %arrayidx.i.phi.trans.insert = getelementptr inbounds i8, i8* %0, i32 %inc.i
  %.pre = load i8, i8* %arrayidx.i.phi.trans.insert, align 1, !tbaa !3
  br label %for.body.i

for.body:                                         ; preds = %if.then, %for.body
  %ptr.011 = phi i8* [ %incdec.ptr, %for.body ], [ %0, %if.then ]
  %i.010 = phi i32 [ %inc, %for.body ], [ 0, %if.then ]
  %inc = add nuw nsw i32 %i.010, 1
  %incdec.ptr = getelementptr inbounds i8, i8* %ptr.011, i32 1
  %3 = load i8, i8* %incdec.ptr, align 1, !tbaa !3
  %cmp1 = icmp eq i8 %3, 0
  br i1 %cmp1, label %for.body.i, label %for.body

if.end:                                           ; preds = %for.inc.i, %if.then, %entry
  ret i32 1
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
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!7, !7, i64 0}
!7 = !{!"any pointer", !4, i64 0}
