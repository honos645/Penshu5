--
-- create all table
--autor:67200072_usami

-- スキーマ作成
CREATE SCHEMA cmss;

-- スキーマ検索パスを変更
SET search_path to cmss;

\i ./admission.sql
\i ./classification_name.sql
\i ./entrance.sql
\i ./entrance_route.sql
\i ./grade_judge.sql
\i ./route_industry.sql
\i ./route_occupation.sql
\i ./route.sql
\i ./subject_classification_advance.sql
\i ./subject_classification_graduate.sql
\i ./subject_detail.sql
\i ./subject_grade.sql
\i ./users.sql
\i ./work.sql
