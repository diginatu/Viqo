Viqo
====

Viqo is a cross-platform "Nico Nama" comment viewer.

Viqo 是跨平台"nico生放"留言查看程序。

Viqo はクロスプラットフォームなニコニコ生放送用 コメントビューア（ニコ生 コメビュ）です。

* クロスプラットフォーム GUI アプリケーション
* Qt5 / C++11
* オープンソース (MIT license)

このコメビュは *自由* であることを目指しています。

ダウンロード
------------

### [Version 2.0](https://github.com/diginatu/Viqo/releases/tag/v2.0)

Ubuntu 14.04, Debian 7.7, OSX Yosemite, Windows 7, 8.1 などで動作確認されています。

使い方
------

[詳しくはWikiへ](https://github.com/diginatu/Viqo/wiki)

サイトからURLのアイコンをドロップして受信できます。

ライセンス
----------

MIT ライセンス

注意
----

#### Nov 25, 2014 のコミットからセーブファイルの保存場所が変更されました。
それ以前からのコミットへバージョンアップする場合で以前のセーブファイルを使いたい場合はファイルを移動する必要があります。

 以前のセーブファイルは実行したカレントディレクトリに作成されていました。

 今後のバージョンでは QStandardPaths を使うため、

  * settings.json
  * user.sqlite

 の２ファイルを移動してください。

[QStandardPaths](http://qt-project.org/doc/qt-5/qstandardpaths.html#LocateOption-enum)

"The following table gives examples of paths on different operating systems." 以降のパス参考


### Viqoの目指す自由
 自由なソフトウェアというだけでなく、クロスプラットフォームで沢山の人が使えることや、多くの目的に素早く適応できるようにコマンドを入力できるようにするなどといった、使用面での自由度など、あらゆる自由を目標にしています。


コンパイル
----------

Qt 5.4 の環境でこのプロジェクトをコンパイルしてください。
実行はできた実行ファイルを実行するだけです。

コントリビュート
----------------

小さなバグ修正から機能追加など、プルリクエストは歓迎します。

使い方・解説を書いてくれる方がいると嬉しいです。

### 現在コントリビュートしやすいリスト

+ [obs-rtmp-nicolive](https://github.com/raccy/obs-rtmp-nicolive/blob/master/doc/PROTOCOL.md)とTCP通信するクラス
+ タブ幅・windowサイズ保存 (状態セーブで)
+ MacのDock対応 (何かよくわからんけどやってください) アイコンとか
+ Wiki の執筆

