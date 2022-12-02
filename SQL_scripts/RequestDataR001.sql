SELECT t.task_id,
       MAX(to_char(DECODE(FIELDCODE, 'BIRTHDAY', f.value))) AS BIRTHDAY,
       MAX(to_char(DECODE(FIELDCODE, 'FIRSTNAME', f.value))) AS NAME,
       MAX(to_char(DECODE(FIELDCODE, 'GIVENNAME', f.value))) AS MIDDLENAME,
       MAX(to_char(DECODE(FIELDCODE, 'LASTNAME', f.value))) AS LASTNAME,
       MAX(to_char(DECODE(FIELDCODE, 'SERIES', f.value))) AS DOCSERIES,
       MAX(to_char(DECODE(FIELDCODE, 'NUMBER', f.value))) AS DOCNUMBER,
       MAX(to_char(DECODE(FIELDCODE, 'ISSUEDATE', f.value))) AS DOCISSUEDATE,
       MAX(to_char(DECODE(FIELDCODE, 'TYPE', f.value))) AS DOCTYPE,
       MAX(to_char(DECODE(FIELDCODE, 'CODE', f.value))) AS REGION,
       t.regioncode
  FROM intask t,
       intaskrequestfield f
WHERE t.SERVICECODE = 'R001'
  AND f.TASK_ID = t.TASK_ID
  AND t.laststatecode = 'I020'
  AND t.createddt >= :date_from
  AND t.createddt <= :date_to
  AND t.regioncode = :region_code
--  AND t.task_id in ('xxxxxxxxxx')
GROUP BY t.task_id, t.regioncode
ORDER BY t.task_id, t.regioncode;