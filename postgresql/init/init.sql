--
-- create all table
--autor:67200072_usami

-- スキーマ作成
CREATE SCHEMA cmss;

--スキーマ変更
SET search_path to cmss;

--サンプルデータ用テーブル作成
\i /table/admission.sql
\i /table/classification_name.sql
\i /table/entrance.sql
\i /table/entrance_route.sql
\i /table/grade_judge.sql
\i /table/route_industry.sql
\i /table/route_occupation.sql
\i /table/route.sql
\i /table/subject_classification_advance.sql
\i /table/subject_classification_graduate.sql
\i /table/subject_grade.sql
\i /table/subject_detail.sql
\i /table/users.sql
\i /table/work.sql

--サンプルデータ登録
\COPY admission FROM /data/admission.csv DELIMITER AS ','
\COPY classification_name FROM /data/classification_name.csv DELIMITER AS ','
\COPY entrance FROM /data/entrance.csv DELIMITER AS ','
\COPY entrance_route FROM /data/entrance_route.csv DELIMITER AS ','
\COPY grade_judge FROM /data/grade_judge.csv DELIMITER AS ','
\COPY route_industry FROM /data/route_industry.csv DELIMITER AS ','
\COPY route_occupation FROM /data/route_occupation.csv DELIMITER AS ','
\COPY route FROM /data/route.csv DELIMITER AS ','
\COPY subject_classification_graduate FROM /data/subject_classification_graduate.csv DELIMITER AS ','
\COPY users FROM /data/users.csv DELIMITER AS ','
\COPY subject_grade FROM /data/subject_grade.csv DELIMITER AS ','
\COPY subject_detail FROM /data/subject_detail.csv DELIMITER AS ','
\COPY subject_classification_advance FROM /data/subject_classification_advance.csv DELIMITER AS ','
\COPY work FROM /data/work.csv DELIMITER AS ','
