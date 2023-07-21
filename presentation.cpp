#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <iomanip>
#include <math.h>
#include <sstream>
#include<emscripten.h>
using namespace std;

vector<string> unknown_values;
class Indivisual
{
public:
    string specific_term;
    string unknown_value;
    double coefficient;
    string operator_sign;

    Indivisual(string Trm, string sign)
    {
        specific_term = Trm;
        operator_sign = sign;
        int n;
        for (n = 0; n < specific_term.length(); n++)                           //function to differentiate unknown values and coefficeints
        {
            if (isalpha(specific_term[n]))
                break;
        }
        coefficient = atoi(specific_term.substr(0, n).c_str());
        if (n == 0)
            coefficient = 1;
        unknown_value = specific_term.substr(n, 1);
        int flag = 0;                                                              //adding unknown values to our vector
        if (unknown_value != "")
        {
            for (int n = 0; n < unknown_values.size(); n++)
            {
                if (unknown_values[n] == unknown_value)
                {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0)
                unknown_values.push_back(unknown_value);
        }
    }
};
class Each_side
{
public:
    vector<Indivisual> tempo_terms;                   
    vector<Indivisual> current_terms;
    void SetSide(string side)
    {
        string Current_term = "";
        string TmpOper = "+";  
        int n;
        for (n = 0; n < side.length(); n++)
        {
            if (side.substr(n, 1) == "+" || side.substr(n, 1) == "-")                      //function to split side into terms
            {
                if (Current_term != "")
                {
                    cout << TmpOper << Current_term << endl;
                    tempo_terms.push_back(Indivisual(Current_term, TmpOper));       //instance from the class terms to define the coefficient, unknowns and signs of our term
                }
                TmpOper = side.substr(n, 1);
                Current_term = "";
            }
            else
                Current_term = Current_term + side.substr(n, 1);
        }
        if (n == side.length())
        {
            if (Current_term != "")
            {
                cout << TmpOper << Current_term << endl;
                tempo_terms.push_back(Indivisual(Current_term, TmpOper));
            }
        }
    }
    void operate_on_similar()
    {
        int flag;
        for (int j = 0; j < tempo_terms.size(); j++)
        {
            flag = 0;
            for (int n = 0; n < current_terms.size(); n++)
            {
                if (current_terms[n].unknown_value == tempo_terms[j].unknown_value)
                {
                    double c = current_terms[n].coefficient;
                    double t = tempo_terms[j].coefficient;
                    if (current_terms[n].operator_sign == "-")
                        c = -1 * current_terms[n].coefficient;
                    if (tempo_terms[j].operator_sign == "-")
                        t = -1 * tempo_terms[j].coefficient;
                    current_terms[n].coefficient = c + t;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0)
                current_terms.push_back(Indivisual(tempo_terms[j].specific_term, tempo_terms[j].operator_sign));
        }
    }
};

class Split_side 
{
public:
    Each_side T_LHS, T_RHS, left_hand_side, right_hand_side;
    Split_side(string Eqn)
    {
        int index = Eqn.find("=");
        cout << "Left hand side: " << Eqn.substr(0, index) << endl;
        cout << "Right hand side : " << Eqn.substr(index + 1) << endl;                        //function to split sides
        cout << "Indivisual terms of the left hand side are: " << endl;
        T_LHS.SetSide(Eqn.substr(0, index));
        cout << "Indivisual terms of the right hand side are: " << endl;
        T_RHS.SetSide(Eqn.substr(index + 1));
        string Oper = "";
         for (int k = 0; k < T_RHS.tempo_terms.size(); k++)
        {
            if (T_RHS.tempo_terms[k].unknown_value != "")   //if the right hand side has unknown, 
            {
                if (T_RHS.tempo_terms[k].operator_sign == "-")
                    Oper = "+";                                       //change side and sign
                else if (T_RHS.tempo_terms[k].operator_sign == "+")
                    Oper = "-";
                left_hand_side.tempo_terms.push_back(Indivisual(T_RHS.tempo_terms[k].specific_term, Oper));//add it to the new right hand side
            }
            else
            {
                right_hand_side.tempo_terms.push_back(Indivisual(T_RHS.tempo_terms[k].specific_term, T_RHS.tempo_terms[k].operator_sign));
            }
        }
        for (int n = 0; n < T_LHS.tempo_terms.size(); n++)
        {
            if (T_LHS.tempo_terms[n].unknown_value == "")               //if the left hand side does not have any unknown
            {
                if (T_LHS.tempo_terms[n].operator_sign == "-")                   //changing sides and operator signs
                    Oper = "+";
                else if (T_LHS.tempo_terms[n].operator_sign == "+")
                    Oper = "-";
                right_hand_side.tempo_terms.push_back(Indivisual(T_LHS.tempo_terms[n].specific_term, Oper));//add it to the new right hand side
            }
            else
                left_hand_side.tempo_terms.push_back(Indivisual(T_LHS.tempo_terms[n].specific_term, T_LHS.tempo_terms[n].operator_sign));
        }
       
        left_hand_side.operate_on_similar();
        right_hand_side.operate_on_similar();
    }
};
vector<Split_side> equation;

vector<vector<double>> CoeffsMatrix;
vector<double> RHSMatrix;

class Matrix_operation
{
public:
    static void form_matrix()
    {
        for (int n = 0; n < equation.size(); n++)
        {
            vector<double> coef;
            double c;
            for (int j = 0; j < unknown_values.size(); j++)
            {
                int flag = 0;
                for (int k = 0; k < equation[n].left_hand_side.current_terms.size(); k++)
                {
                    if (unknown_values[j] == equation[n].left_hand_side.current_terms[k].unknown_value)
                    {
                        c = equation[n].left_hand_side.current_terms[k].coefficient;
                        if (equation[n].left_hand_side.current_terms[k].operator_sign == "-")
                            c = -1 * equation[n].left_hand_side.current_terms[k].coefficient;
                        coef.push_back(c);
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0)
                    coef.push_back(0);
            }
            CoeffsMatrix.push_back(coef);
            c = equation[n].right_hand_side.current_terms[0].coefficient;
            if (equation[n].right_hand_side.current_terms[0].operator_sign == "-")
                c = -1 * equation[n].right_hand_side.current_terms[0].coefficient;
            RHSMatrix.push_back(c);
        }
    }
    static vector<vector<double>> variable_matrix(int var, vector<vector<double>> CoeffMatrix, vector<double> RhsMatrix)
    {
        for (int n = 0; n < CoeffMatrix.size(); n++)
        {
            CoeffMatrix[n][var] = RhsMatrix[n];
        }
        return CoeffMatrix;
    }
   
    static void DisplayMatrix(vector<vector<double>> matrix)
    {
        for (int n = 0; n < matrix.size(); n++)
        {
            for (int j = 0; j < matrix[n].size(); j++)
            {
                cout << matrix[n][j] << " ";
            }
            cout << endl;
        }
    }
    static int find_determinant(vector<vector<double>> Matrix_operation)
    {
        double det = 0;
        if (Matrix_operation.size() == 1)
        {
            return Matrix_operation[0][0];
        }
        else
        {
            det = (Matrix_operation[0][0] * Matrix_operation[1][1] - Matrix_operation[0][1] * Matrix_operation[1][0]);
            return det;
        }
    }
};



extern "C"


{
    EMSCRIPTEN_KEEPALIVE
    void PassEquations(char *Eqn)// character pointer beacuse js cannot understand string in c++
    {
        
        string Eqnx(Eqn);//convert the character pointer into string
        vector<string> EquationText; //seperate the string from the semicolon character 
        while (true)
        {
            int SeparatorIndex = Eqnx.find(";");
            if (SeparatorIndex != -1)
            {
                EquationText.push_back(Eqnx.substr(0, SeparatorIndex)); //first eq added to the vector>>remove and iterate for second
                Eqnx = Eqnx.substr(SeparatorIndex + 1); 
            }
            else
                break; 
        }
        for (int n = 0; n < EquationText.size(); n++) //then we will iterate on each equation
        {
            equation.push_back(Split_side(EquationText[n]));//create instance from the equation class and pass to it 
            //the equation string then add the object to Equations vector
        }
       
    }
}
EMSCRIPTEN_KEEPALIVE 
int main(int argc, char const *argv[])      
{ 
    int argv_count = atoi(argv[1]);
    if (argv_count < 2)
        return 0;
    for (int n = 0; n < argv_count; n++)                          //taking input
    {
        string Eqn = "";
        cout << "Enter Equation " << (n + 1)<< endl;
        cin >> Eqn;
        equation.push_back(Split_side(Eqn));
    }
   
    
}