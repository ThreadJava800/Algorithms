Program ggipetro47;

Uses crt; {* Подключаем внешние файлы *}
label l1;
Var {* Объявление переменных *}
  a      : Array[1..50] Of integer;
  b      : Array[1..50] Of integer;
  i      : byte;
  j      : byte;
  kub    : byte;
  cvt    : byte;
  nowkub : byte;
  kol    : byte;
  raz    : byte;
  ok     : boolean;
Begin
  clrscr; {* Очистка экрана *}
  textcolor(yellow); {*  текст *}
  write('Kolichestvo kubikov: ');
  readln(kub);
  write('Kolichestvo cvetov: ');
  readln(cvt);
  clrscr; {* Очищаем экран *}
  For i := 1 To kub Do {ruchnoe zapolnenie}
  Begin
    l1 : write('Cvet ', i, '-go kubika (1-', cvt, '): ');
    readln(a[i]);
    clrscr; {* Очистка экрана *}
    If (a[i]>cvt) Then
    Begin
      writeln('Nevernii cvet!');
      Goto l1;
    End;
    
  End;
  write('> ');
  textcolor(white); {*  текст *}
  For i := 1 To kub Do {* Переменная i увеличивается с 1 до kub *}
    write(a[i], ' '); {raspechatka}
  nowkub := round(kub / 2) + 1;
  For i := 1 To nowkub Do {perebor vozmognih znachenii}
  Begin
    ok := false;
    raz := i - 1;
    For j := 1 To raz Do 
      If a[i + j - 1]>=a[i - j] Then
      Begin
        ok := true;
      End;
    If ok=true Then {zapisivanie dopolnitelnih dannih}
    Begin
      inc(kol); {* kol++ *}
      b[kol] := kub - raz;
    End;
    
  End;
  writeln; {* Пустая строка *}
  For i := 1 To kol Do {vivod itoga}
  Begin
    writeln; {* Пустая строка *}
    textcolor(yellow); {* Выбрать  цвет текста *}
    write('> ');
    For j := 1 To kub Do {* Цикл для j => [1 .. kub] *}
    Begin
      If kub - b[i] = j - 1 Then
      Begin
        textcolor(yellow); {* Установить  цвет текста *}
        write('| ');
      End;
      textcolor(white); {* Установить  цвет текста *}
      write(a[j], ' ');
    End;
    write('..Kubikov: ', b[i], '..');
  End;
  readkey; {* Ждем нажатия любой клавиши *}
End.
