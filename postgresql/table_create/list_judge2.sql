-- 判定の一覧表示(学科長)
-- 67200072_宇佐見
SELECT
  grade_judge.id,
  users.person_name,
  CASE
    WHEN grade_judge.completion_judge = 0 THEN '可'
    ELSE '不可'
  END AS judge
FROM
  grade_judge
  INNER JOIN
    users
    ON
    grade_judge.id = users.id
WHERE
  (
    grade_judge.id LIKE 'B%'
    OR grade_judge.id LIKE 'M%'
    OR grade_judge.id LIKE 'D%'
  )
;