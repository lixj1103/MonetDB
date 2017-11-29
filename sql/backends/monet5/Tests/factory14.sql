--Update the arguments during
CREATE FUNCTION factory14(aa INT, bb CLOB) RETURNS TABLE (aa INT, bb CLOB) BEGIN
    YIELD TABLE (SELECT aa, bb);
    SET aa = aa + 1;
    SET bb = 'just';
    YIELD TABLE (SELECT aa, bb);
    SET aa = aa + 1;
    SET bb = 'other';
    YIELD TABLE (SELECT aa, bb);
    SET aa = aa + 1;
    SET bb = 'string';
    YIELD TABLE (SELECT aa, bb);
END;

SELECT aa, bb FROM factory14(0, '');
SELECT aa, bb FROM factory14(1, 'first');
SELECT aa, bb FROM factory14(2, 'second');
SELECT aa, bb FROM factory14(3, 'third');
SELECT aa, bb FROM factory14(4, 'fourth'); --error

DROP FUNCTION factory14;