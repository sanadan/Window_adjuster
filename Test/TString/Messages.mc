;///
;///	@file	Messages.mc
;///	@brief	メッセージ定義ファイル
;///	@note	メッセージファイルをコンパイルする場合は以下の
;///			コマンドを使用する。\n
;///				mc -c -u Messages.mc
;///
FacilityNames =
(
	TStringTest = 0x000
)
LanguageNames = ( Japanese = 0x411:MSG00411 )
MessageIdTypedef = DWORD

;
; // テスト用メッセージフォーマット
;
MessageID = 1
SymbolicName = TSTRING_TEST_FORMAT1
Facility = TStringTest
Severity = Informational
Language = Japanese
%2 / %1%0
.
MessageID = +1
SymbolicName = TSTRING_TEST_FORMAT2
Facility = TStringTest
Severity = Informational
Language = Japanese
%2／%1%0
.
