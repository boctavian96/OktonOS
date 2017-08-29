{OKTON Console}

{program Okton_Console;}

unit console;

interface

var 
    xpos : Integer = 0;
    ypos : Integer = 0;
    videoMemory : PChar = PChar($b8000);

procedure kclearscreen();
procedure kwritechr(c : char);
procedure kwritestr(strng : PChar);
procedure kwriteint(i : integer);
procedure kwritedword(i : DWORD);

implementation

procedure kclearscreen(); [public, alias: 'kclearscreen'];
var 
    i : integer;
begin 
    for i := 0 to 3999 do 
        videoMemory[i] := #0;
end;

procedure kwritechr(c : char); [public, alias: 'kwritechr'];
var 
    offset : integer;

begin

    if(ypos > 24) then 
        ypos := 0;
    if(xpos > 79) then
        xpos := 0;

    offset := (xpos shl 1) + (ypos * 160);
    videoMemory[offset] := c;
    inc(offset);
    videoMemory[offset] := #7;
    inc(offset);

    xpos := (offset mod 160);
    ypos := (offset - xpos) div 160;
    xpos := xpos shr 1;

end;

procedure kwritestr( strng : PChar); [public, alias: 'kwritestr'];
var
    offset, i : integer;

begin

    if (ypos > 24) then 
        ypos := 0;
    if (xpos > 79) then
        xpos := 0;

    offset := (xpos shl 1) + (ypos * 160);
    i := 0;
    
    while (strng[i] <> Char($0)) do 
    begin 
        videoMemory[offset] := strng[i];
        inc(offset);
        videoMemory[offset] := #7;
        inc(offset);
        inc(i);
    end;

    xpos := (offset mod 160);
    ypos := (offset - xpos) div 160;
    xpos := xpos shr 1;
    
end;

procedure kwriteint(i : integer); [public, alias: 'kwritein'];
var 
    buffer: array[0..11] of char;
    str : PChar;
    digit : DWORD;
    minus : boolean;

begin 
    str := @buffer[11];
    str^ := #0;

    if (i < 0) then 
    begin
        digit := -i;
        minus := true;
    end
    else
    begin
        digit := i;
        minus := false;
    end;

    repeat
        dec(str);
        str^ := Char((digit mod 10) + Byte('0'));
        digit := digit div 10;
    until (digit = 0);

    if (minus) then
    begin 
        dec(str);
        str^ := '-';
    end;

    kwritestr(str);
        
end;

procedure kwritedword(i : DWORD); [public, alias: 'kwritedword'];
var
    buffer : array [0..11] of char;
    str : PChar;
    digit : DWORD;

begin
    for digit := 0 to 10 do 
        buffer[digit] := '0';

    str := @buffer[11];
    str^ := #0;

    digit := i;
    repeat 
        dec(str); {str--}
        str^ := Char((digit mod 10) + Byte('0'));
        digit := digit div 10;
    until(digit = 0);

    kwritestr(@Buffer[0]);
        
end;

end.
