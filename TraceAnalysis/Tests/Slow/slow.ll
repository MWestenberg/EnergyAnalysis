; ModuleID = 'Slow\slow.c'
source_filename = "Slow\5Cslow.c"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i686-pc-windows-msvc19.16.27025"

@.str = private unnamed_addr constant [30 x i8] c"digitalWriteHIGH,0,0,0.004534\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [12 x i8] c"Slow\5Cslow.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [29 x i8] c"digitalWriteLOW,0,0,0.004534\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [12 x i8] c"comp,75,0,0\00", section "llvm.metadata"
@.str.4 = private unnamed_addr constant [11 x i8] c"comp,0,0,0\00", section "llvm.metadata"
@llvm.global.annotations = appending global [4 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_digitalWriteHIGH to i8*), i8* getelementptr inbounds ([30 x i8], [30 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i32 0, i32 0), i32 6 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_digitalWriteLOW to i8*), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i32 0, i32 0), i32 7 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_compOn to i8*), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i32 0, i32 0), i32 8 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_compOff to i8*), i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.4, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i32 0, i32 0), i32 9 }], section "llvm.metadata"

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
define dso_local void @_compOn() #0 {
entry:
  ret void
}

; Function Attrs: norecurse nounwind readnone
define dso_local void @_compOff() #0 {
entry:
  ret void
}

; Function Attrs: nounwind
define dso_local void @bla(i1 zeroext %i) local_unnamed_addr #1 {
entry:
  br i1 %i, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  tail call void (i32, ...) @digitalWriteHIGH(i32 2, i32 1) #3
  br label %if.end

if.else:                                          ; preds = %entry
  tail call void (i32, ...) @digitalWriteLOW(i32 2, i32 0) #3
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret void
}

declare dso_local void @digitalWriteHIGH(i32, ...) local_unnamed_addr #2

declare dso_local void @digitalWriteLOW(i32, ...) local_unnamed_addr #2

; Function Attrs: nounwind
define dso_local i32 @main() local_unnamed_addr #1 {
entry:
  tail call void bitcast (void (...)* @compOn to void ()*)() #3
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %for.cond.cleanup3, %entry
  %i.018 = phi i32 [ 0, %entry ], [ %inc9, %for.cond.cleanup3 ]
  br label %for.body4

for.cond.cleanup:                                 ; preds = %for.cond.cleanup3
  tail call void bitcast (void (...)* @compOff to void ()*)() #3
  ret i32 1

for.cond.cleanup3:                                ; preds = %for.inc
  %inc9 = add nuw nsw i32 %i.018, 1
  %exitcond19 = icmp eq i32 %inc9, 1000
  br i1 %exitcond19, label %for.cond.cleanup, label %for.cond1.preheader

for.body4:                                        ; preds = %for.inc, %for.cond1.preheader
  %x.017 = phi i32 [ 0, %for.cond1.preheader ], [ %inc, %for.inc ]
  %0 = or i32 %x.017, %i.018
  %1 = and i32 %0, 1
  %2 = icmp eq i32 %1, 0
  br i1 %2, label %if.then, label %if.else

if.then:                                          ; preds = %for.body4
  tail call void (i32, ...) @digitalWriteHIGH(i32 2, i32 1) #3
  br label %for.inc

if.else:                                          ; preds = %for.body4
  tail call void (i32, ...) @digitalWriteLOW(i32 2, i32 0) #3
  br label %for.inc

for.inc:                                          ; preds = %if.then, %if.else
  %inc = add nuw nsw i32 %x.017, 1
  %exitcond = icmp eq i32 %inc, 1000
  br i1 %exitcond, label %for.cond.cleanup3, label %for.body4
}

declare dso_local void @compOn(...) local_unnamed_addr #2

declare dso_local void @compOff(...) local_unnamed_addr #2

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{!"clang version 8.0.0 (https://github.com/llvm-mirror/clang.git bfbe338a893dde6ba65b2bed6ffea1652a592819) (https://github.com/llvm-mirror/llvm.git a99d6d2122ca2f208e1c4bcaf02ff5930f244f34)"}
