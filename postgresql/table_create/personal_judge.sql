-- 個人の判定を表示
-- 67200072_宇佐見
SELECT
  grade_judge.id
  ,users.person_name
  ,grade_judge.completion_judge
FROM
  grade_judge
  INNER JOIN users ON grade_judge.id = users.id
WHERE
  grade_judge.id = 'B1120001'
;