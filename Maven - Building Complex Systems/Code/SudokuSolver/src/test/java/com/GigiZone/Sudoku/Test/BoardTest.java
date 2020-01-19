/*
 * BoardTest.java
 * JUnit based test
 *
 * Created on September 11, 2005, 10:23 AM
 */

package com.GigiZone.Sudoku;

import junit.framework.*;

/**
 *
 * @author Gigi
 */
public class BoardTest extends TestCase
{
    private Board board = null;
    public BoardTest(String testName)
    {
        super(testName);
    }

    protected void setUp() throws Exception
    {
        board = new Board();
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                board.setSquare(i, j, j);
    }

    protected void tearDown() throws Exception
    {
    }

    public static Test suite()
    {
        TestSuite suite = new TestSuite(BoardTest.class);

        return suite;
    }

    public void testGetSquare()
    {
        assertEquals(board.getSquare(5, 6), 6);
    }

    /**
     * Test of setSquare method, of class com.GigiZone.Sudoku.Board.
     */
    public void testSetSquare()
    {
        assertEquals(board.getSquare(5, 6), 6);
        board.setSquare(5,6, 2);
        assertEquals(board.getSquare(5, 6), 2);
    }

    /**
     * Test of validateSquare method, of class com.GigiZone.Sudoku.Board.
     */

    /**
     * Test of validate method, of class com.GigiZone.Sudoku.Board.
     */
    public void testValidate()
    {
        assertTrue(new Board().validate());
        assertFalse(board.validate());
    }

    /**
     * Test of validateRow method, of class com.GigiZone.Sudoku.Board.
     */
    public void testValidateRow()
    {
        assertTrue(board.validateRow(0));
        for (int i = 0; i < 9; ++i)
            board.setSquare(0, i, i+1);
        assertTrue(board.validateRow(0));
        board.setSquare(0,0,5);
        board.setSquare(0,1,5);
        assertFalse(board.validateRow(0));
    }

    /**
     * Test of validateColumn method, of class com.GigiZone.Sudoku.Board.
     */
    public void testValidateColumn()
    {
        assertTrue(board.validateColumn(0));
        for (int i = 0; i < 9; ++i)
            board.setSquare(i, 0, i+1);
        assertTrue(board.validateColumn(0));
        board.setSquare(0,1,5);
        board.setSquare(1,1,5);
        assertFalse(board.validateRow(0));

    }

    /**
     * Test of validateSection method, of class com.GigiZone.Sudoku.Board.
     */
    public void testValidateSection()
    {
        assertFalse(board.validateSection(0));
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                board.setSquare(i, j, 0);
        assertTrue(board.validateSection(0));
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                board.setSquare(i, j, i*3+j+1);

        assertTrue(board.validateSection(0));
        board.setSquare(0,0,5);
        board.setSquare(1,1,5);
        assertFalse(board.validateSection(0));

        //assertTrue(board.validateSection(1));
        board.setSquare(0,4,5);
        board.setSquare(1,5,5);
        //assertFalse(board.validateSection(1));

    }

}
