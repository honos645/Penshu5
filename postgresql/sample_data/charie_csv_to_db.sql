--
-- copy CSV file
--autor:67200072_usami


-- スキーマ検索パスを変更
SET search_path to cmss;

\COPY admission FROM ./admission.csv DELIMITER AS ','
\COPY entrance FROM ./entrance.csv DELIMITER AS ','
\COPY entrance_route FROM ./entrance_route.csv DELIMITER AS ','
\COPY grade_judge FROM ./grade_judge.csv DELIMITER AS ','
\COPY route_industry FROM ./route_industry.csv DELIMITER AS ','
\COPY route_occupation FROM ./route_occupation.csv DELIMITER AS ','
\COPY route FROM ./route.csv DELIMITER AS ','
\COPY subject_classification_advance FROM ./subject_classification_advance.csv DELIMITER AS ','
\COPY subject_classification_graduate FROM ./subject_classification_graduate.csv DELIMITER AS ','
\COPY subject_grade FROM ./subject_grade.csv DELIMITER AS ','
\COPY subject_detail FROM ./subject_detail.csv DELIMITER AS ','
\COPY users FROM ./users.csv DELIMITER AS ','
\COPY work FROM ./work.csv DELIMITER AS ','