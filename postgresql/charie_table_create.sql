--
-- create all table
--autor:67200072_usami

-- スキーマ作成
CREATE SCHEMA cmss;

-- スキーマ検索パスを変更
SET search_path to cmss;

\i ~/Penshu5/postgresql/table_create/admission.sql
\i ~/Penshu5/postgresql/table_create/entrance.sql
\i ~/Penshu5/postgresql/table_create/grade_judge.sql
\i ~/Penshu5/postgresql/table_create/route_industry.sql
\i ~/Penshu5/postgresql/table_create/route_occupation.sql
\i ~/Penshu5/postgresql/table_create/route.sql
\i ~/Penshu5/postgresql/table_create/subject_grade.sql
\i ~/Penshu5/postgresql/table_create/subject_detail.sql
\i ~/Penshu5/postgresql/table_create/users.sql
\i ~/Penshu5/postgresql/table_create/work.sql
