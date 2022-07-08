--
-- copy CSV file
--autor:67200072_usami


-- スキーマ検索パスを変更
SET search_path to cmss;

\COPY admission FROM ~/Penshu5/postgresql/sample_data/admission.csv DELIMITER AS ','
\COPY entrance FROM ~/Penshu5/postgresql/sample_data/entrance.csv DELIMITER AS ','
\COPY grade_judge FROM ~/Penshu5/postgresql/sample_data/grade_judge.csv DELIMITER AS ','
\COPY route_industry FROM ~/Penshu5/postgresql/sample_data/route_industry.csv DELIMITER AS ','
\COPY route_occupation FROM ~/Penshu5/postgresql/sample_data/route_occupation.csv DELIMITER AS ','
\COPY route FROM ~/Penshu5/postgresql/sample_data/route.csv DELIMITER AS ','
\COPY subject_grade FROM ~/Penshu5/postgresql/sample_data/subject_grade.csv DELIMITER AS ','
\COPY subject_detail FROM ~/Penshu5/postgresql/sample_data/subjectDetail.csv DELIMITER AS ','
\COPY users FROM ~/Penshu5/postgresql/sample_data/users.csv DELIMITER AS ','
\COPY work FROM ~/Penshu5/postgresql/sample_data/work.csv DELIMITER AS ','


