; ModuleID = 'SoundSystem\test1.c'
source_filename = "SoundSystem\5Ctest1.c"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i686-pc-windows-msvc19.16.27025"

@.str = private unnamed_addr constant [28 x i8] c"SoundSystem, 1000,1000,1000\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [23 x i8] c"SoundSystem/settings.h\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [22 x i8] c"SoundSystem, 0,0,1000\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [24 x i8] c"PlayBeepAtHz,0,100,1000\00", section "llvm.metadata"
@.str.4 = private unnamed_addr constant [16 x i8] c"delay, 0,0,5000\00", section "llvm.metadata"
@llvm.global.annotations = appending global [4 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_SoundSystemOn to i8*), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i32 0, i32 0), i32 2 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_SoundSystemOff to i8*), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i32 0, i32 0), i32 3 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_PlayBeepAtHz to i8*), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i32 0, i32 0), i32 4 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void ()* @_delay to i8*), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.4, i32 0, i32 0), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i32 0, i32 0), i32 5 }], section "llvm.metadata"

; Function Attrs: norecurse nounwind readnone
define dso_local void @_SoundSystemOn() #0 {
entry:
  ret void
}

; Function Attrs: norecurse nounwind readnone
define dso_local void @_SoundSystemOff() #0 {
entry:
  ret void
}

; Function Attrs: norecurse nounwind readnone
define dso_local void @_PlayBeepAtHz() #0 {
entry:
  ret void
}

; Function Attrs: norecurse nounwind readnone
define dso_local void @_delay() #0 {
entry:
  ret void
}

; Function Attrs: nounwind
define dso_local i32 @main(i32 %argc, i8** nocapture readnone %argv) local_unnamed_addr #1 {
entry:
  %cmp = icmp sgt i32 %argc, 1
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  tail call void bitcast (void (...)* @SoundSystemOn to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @PlayBeepAtHz to void ()*)() #3
  tail call void bitcast (void (...)* @delay to void ()*)() #3
  tail call void bitcast (void (...)* @SoundSystemOff to void ()*)() #3
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  ret i32 1
}

declare dso_local void @SoundSystemOn(...) local_unnamed_addr #2

declare dso_local void @PlayBeepAtHz(...) local_unnamed_addr #2

declare dso_local void @delay(...) local_unnamed_addr #2

declare dso_local void @SoundSystemOff(...) local_unnamed_addr #2

attributes #0 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{!"clang version 8.0.0 (https://github.com/llvm-mirror/clang.git bfbe338a893dde6ba65b2bed6ffea1652a592819) (https://github.com/llvm-mirror/llvm.git a99d6d2122ca2f208e1c4bcaf02ff5930f244f34)"}
