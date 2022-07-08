# docker-composeのコマンドまとめ

# フロントで起動
check:
	docker-compose -f ./postgresql/docker-compose.yml up --build

# バックグラウンドで起動
up:
	docker-compose -f ./postgresql/docker-compose.yml up -d --build

# 確認
ps:
	docker-compose -f ./postgresql/docker-compose.yml ps

# 停止
stop:
	docker-compose -f ./postgresql/docker-compose.yml stop

# 削除
down:
	docker-compose -f ./postgresql/docker-compose.yml down

# コンテナに接続
db:
	docker-compose -f ./postgresql/docker-compose.yml exec db /bin/bash

# サーバーに接続
dev_c:
	docker-compose -f ./postgresql/docker-compose.yml exec c_dev /bin/bash
