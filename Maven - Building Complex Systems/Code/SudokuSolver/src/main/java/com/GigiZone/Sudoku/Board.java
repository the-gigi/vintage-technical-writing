package com.GigiZone.Sudoku;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class Board implements Serializable
{
    public Board()
    {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
            {
                m_initialBoard[i][j] = 0;
                m_board[i][j] = 0;
            }
    }

    public Board(Board other)
    {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
            {
                m_initialBoard[i][j] = other.m_initialBoard[i][j];
                m_board[i][j] = other.m_board[i][j];  ;
            }
    }

    public Board(String s)
    {
        assignInitialString(s);
        assignString(s);
    }

    public void assignString(String s)
    {
        assert(s.length() == 81);
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
            {
                char c = s.charAt(i*9+j);
                assert(c <= '9' && c >= '0');
                int value = c - '0';
                m_board[i][j] = value;
            }
    }

    public void assignInitialString(String s)
    {
        assert(s.length() == 81);
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
            {
                char c = s.charAt(i*9+j);
                assert(c <= '9' && c >= '0');
                int value = c - '0';
                m_initialBoard[i][j] = value;
            }
    }

    public int getSquare(int row, int col)
    {
        return m_board[row][col];
    }

    public int getInitialSquare(int row, int col)
    {
        return m_initialBoard[row][col];
    }

    public void setSquare(int row, int col, int value)
    {
        m_board[row][col] = value;
    }

    public String getName()
    {
        return m_name;
    }

    public void setName(String name)
    {
        m_name = name;
    }

    public String getString()
    {
        return m_string;
    }

    public void setString(String s)
    {
        m_string = s;
    }

    public boolean validate()
    {
        for (int i = 0; i < 9; ++i)
        {
            if (!validateRow(i)    ||
                !validateColumn(i) ||
                !validateSection(i))
                return false;
        }
        return true;
    }

    public boolean validateRow(int row)
    {
        int found[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        for (int i = 0; i < 9; ++i)
        {
            int value = m_board[row][i];
            if (value == 0) // ignore empty squares
                continue;
            if (found[value-1] > 0)
                return false;
            else
                found[value-1] = 1;
        }
        return true;
    }

    public boolean validateColumn(int col)
    {
        int found[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        for (int i = 0; i < 9; ++i)
        {
            int value = m_board[i][col];
            if (value == 0) // ignore empty squares
                continue;
            if (found[value-1] > 0)
                return false;
            else
                found[value-1] = 1;
        }
        return true;
    }

    public Boolean getDone()
    {
//        return true;
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                if (m_board[i][j] == 0)
                    return false;
        return getValid();
    }

    public Boolean getValid()
    {
        return validate();
    }

    public boolean validateSection(int section)
    {
        int found[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        int i_offset = (section / 3) * 3;
        int j_offset = (section % 3) * 3;
        for (int i = i_offset; i < i_offset+3; ++i)
            for (int j = j_offset; j < j_offset+3; ++j)
            {
                int value = m_board[i][j];
                if (value == 0) // ignore empty squares
                    continue;
                if (found[value-1] > 0)
                    return false;
                else
                    found[value-1] = 1;
            }

        return true;
    }


    private int[][] m_board = new int[9][9];
    private int[][] m_initialBoard = new int[9][9];
    private String  m_name;
    private String  m_string = null;
}
