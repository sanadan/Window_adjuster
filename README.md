# Window adjuster
ウィンドウ調整ツール Version 0.5.0

## 概要
　このプログラムはウィンドウの位置を画面の端に合わせたり、大きさを画面の
縦横半分／縦最大化／横最大化などに調整するツールです。

　また、ウィンドウクラス単位のウィンドウ位置とアイコン名単位のデスクトッ
プアイコン位置を、画面サイズ毎に保存／復元することができます。

## 動作確認環境
* Windows 7 Home Premium SP1 (64bit)
* Windows 8.1 Pro (64bit)

## インストール
1. 適当なフォルダに Window_adjuster.exe と WA_Hook.dll をコピーしてくだ
   さい。
   （c:\Program Files\Window adjuster 等）
2. Window_adjuster.exeを起動してください。
3. タスクトレイに Window adjuster のアイコンが表示されれば完了です。
4. 必要に応じて、スタートアップフォルダにショートカットを作成したりして
   ください。
5. 「MSVCP120.DLL がないため、プログラムを開始できません。」というエラー
   が表示される場合は、以下のランタイムをインストールしてください。
   http://www.microsoft.com/ja-jp/download/details.aspx?id=40784

## アンインストール
1. タスクトレイの Window adjuster アイコンを右クリックしてください。
2. 「終了」を選択してください。
3. タスクトレイのアイコンが消えたら、Windows_adjuster.exe と WA_Hook.dll
   を削除してください。特にレジストリは使用していません。
4. ウィンドウ位置保存機能を使った場合、
   %APPDATA%\Javelin_Software\Windows_adjuster フォルダ配下に位置情報ファ
   イルが残るので、必要に応じて削除してください。

## 使い方
1. ウィンドウの「閉じる」ボタンを右クリックすると、Adjustダイアログが表
   示されます。各ボタンで位置や大きさを調整します。「OK」ボタンでその位
   置／大きさで決定、「キャンセル」ボタンで元の位置に戻ります。
2. ウィンドウタイトルでCtrl+中央クリックすると、1)と同じ動作をします。
3. ウィンドウの「最大化」ボタンを右クリックすると、縦横が画面の半分にな
   ります。その状態で再度「最大化」ボタンを右クリックすると、縦が最大化
   されます。
4. ウィンドウの枠でダブルクリックすると、その方向の画面の端に移動します。
  （例：右上でダブルクリック→右上に寄せられる）
5. ウィンドウの枠で右ダブルクリックすると、その方向に最大化されます。
  （例：右端で右ダブルクリック→横最大化）
6. タスクトレイアイコンを右クリックして「配置情報保存」を選択すると、現
   在の画面サイズでのウィンドウ位置とデスクトップアイコン位置を保存しま
   す。なお、最小化や非表示のウィンドウは保存対象外になります。
7. タスクトレイアイコンを右クリックして「配置情報復元」を選択するか、タ
   スクトレイアイコンをダブルクリックすると、現在の画面サイズでのウィン
   ドウ位置とデスクトップアイコン位置を復元します。なお、最小化や非表示
   のウィンドウは復元対象外になります。
8. タスクトレイアイコンを右クリックして「配置情報消去」を選択すると、現
   在の画面サイズでのウィンドウ位置とデスクトップアイコン位置の情報を消
   去します。この操作後、再度「配置情報保存」で、対象ウィンドウを設定し
   直すことができます。

## 改版履歴
* 0.5.0 (2014/10/21) :64bit対応
* 0.4.0 (2011/12/09) :ウィンドウ／デスクトップアイコンの位置保存／復元機能追加
* 0.2.0 (2008/11/16) :ウィンドウの操作方法を変更
* 0.1.0 :  試作版

## 連絡先
　ご意見、ご要望、バグレポート等は、

    jecy00@gmail.com

まで。
