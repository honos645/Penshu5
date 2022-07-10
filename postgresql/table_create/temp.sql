SELECT
  subject_grade.subject_code,
  subject_detail.subject_name,
  subject_grade.opening_year,
  subject_detail.credit,
  subject_detail.classification,
  subject_grade.grade_point
FROM
  subject_grade
  INNER JOIN subject_detail ON subject_grade.subject_code = subject_detail.course_sharing_code
WHERE
  subject_grade.id = 'B1120001';


SELECT
  subject_detail.subject_code,
  subject_detail.course_sharing_code,
  subject_detail.subject_name,
  classification_name.classification_name,
  classification_name.classification_code
FROM
  subject_detail
  INNER JOIN classification_name
  ON subject_detail.classification = classification_name.classification_code
  ;

SELECT
  classification,
  count(classification)
FROM
  subject_detail
GROUP BY
  classification;


SELECT
  grade_judge.id
  , users.person_name
  , CASE
WHEN grade_judge.research_judge = 0
THEN '可' WHEN grade_judge.research_judge = 1
THEN '不可' ELSE '未'
END AS judge
FROM
  grade_judge
INNER JOIN users
ON
  grade_judge.id=users.id
WHERE
  grade_judge.id LIKE 'B%'
OR
  grade_judge.id LIKE 'M%'
OR grade_judge.id LIKE 'D%';