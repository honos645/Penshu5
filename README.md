# Penshu5group
* 最終的な実行ファイル名はPMSS (Promotion Management Support Systemの略)
* データベースのスキーマ名はCMSS (Charlie Management Support Systemの略)

## Dockerについて
[ここ](https://github.com/honos645/Penshu5.git)にあります。ホームディレクトリに置いてください。
postgresqlディレクトリでpsqlすると一括でテーブルやデータの登録ができるcharlie~.sqlが使えます。
Dockerの環境が手元にある場合、Penshu5ディレクトリ以下のコマンドを実行.
 * `make up` 起動、自動でテーブルを作成
 * `make db` postgresqlにアクセス(中でpsql ~ を打つ) デフォルトはhost:kite, user:dbuser06, db:db06, pass:dbpass06
 * `make serv` C言語の環境。エディターはVim Emacs nano micro ここでサーバーを起動するとport1000のlocalhostのIPアドレスで接続可能
 * `make down` すべて終了

## ユーザレベル
|ユーザーレベル||
|---:|:---:|
|管理者|0|
|学生|1|
|事務員|2|
|教務委員|3|
|担任なし|4|
|学年担任|5|
|学年副担任|6|
|就職担当教員-担任あり|7|
|就職担当教員-担任なし|8|
|学科(プログラム)長|9|
## 学籍番号規則
* 全8桁
* 左から1桁目:役職
* 左から2桁目:学部
* 左から3桁目:学科・プログラム
* 左から4,5桁目:入学年度
* 左から6,7,8桁目:出席番号



|役職||学部||学科・プログラム(工学部のみ)|2020以前|学科・プログラム(工学部のみ)|2021以降|学科・プログラム(工学部のみ)|院のコース|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|学部生|B|管理者|0|管理者|0|管理者|0|管理者|0|
|修士|M|工学|1|情報システム工学|1|情報通信工学|1|環境応用化学モデル|1|
|博士|D|教育|2|電気システム工学|2|電気電子工学|2|社会環境工学モデル|2|
|事務|S|農学|3|電子物理工学|3|応用物理工学|3|材料開発モデル|3|
|教員|T|地域資源|4|社会環境システム工学|4|土木環境工学|4|電気電子モデル|4|
|管理者|A|医学|5|環境応用化学|5|応用物質化学|5|エネルギーサイエンスモデル|5|
|||||機械設計システム工学|6|機械知能工学|6|応用数学モデル|6|
|||||環境ロボティクス|7|||機械システムモデル|7|
|||||||||??ロボティクスモデル|8|
|||||||||情報システムモデル|9|


### 具体例
* B1120007->2020年入学工学部情報システム工学科の出席番号007の学生
* A0000000->管理者

## 判定
進級，卒業研究，卒業，修了の判定はint型で４つの値
<!-- NOTE:合格と不合格の値逆にならないかな? -->
|合|否|保留|未設定|
|:---:|:---:|:---:|:---:|
|0|1|2|-1|

## 科目詳細
* 科目コードはシーケンス
* 開講年度は年度(4桁)+前期(0)or後期(1)の5桁
* 科目共有コードは共通する講義で最も古い講義の科目共有コードを割り当て
* 所定外科目は自学科以外の科目？
* 履修可能学科は学籍番号の上3桁+改組回数(例:2020年度以前は0,それ以降は1)
* necessaryは必修科目の場合0,選択科目の場合1

|区分コード|区分|
|:---:|:---:|
|10|基礎教育必修|
|11|大学教育入門セミナー|
|12|情報数量スキル|
|13|外国語コミュニケーション(英語)|
|14|外国語コミュニケーション(初修外国語)|
|15|保健体育|
|16|専門基礎|
|17|専門教育入門セミナー|
|18|環境と生命|
|20|現代社会の課題|
|21|社会と人間|
|22|自然の仕組み|
|30|学士力発展科目|
|31|地域・学際系|
|32|自然科学系|
|33|外国語系|
|41|留学生対象科目|
|50|工学科目必修|
|51|工学基礎必修|
|52|専門必修科目|
|60|工学科目選択|
|61|工学基礎選択|
|62|専門選択科目|
|71|専門科目(その他)|
|81|教職科目|
|91|自由選択科目|
|101|所定外科目|

## 科目成績
* 科目コードはシェアコードではない?
* 開講年度は科目詳細と同じ規則
* idは学籍番号
* grade_pointは成績。詳細は下の表

|点数|意味|
|:---:|:---:|
|0~59|否|
|60|可(再)|
|60~69|可|
|70~79|良|
|80~89|優|
|90~100|秀|
|333|未受験|
|666|履修中|
|999|出席不足|

## ユーザ情報
<!-- NOTE:パスワードは数字のみ? -->
<!-- NOTE:学部学科は何故char型？ -->

## 就職関連
<!-- NOTE:就職先分類のカラムが必要? -->
