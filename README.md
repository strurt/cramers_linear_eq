# cramers_linear_eq
the flow of the code is as follows:
> Include necessary header files for input/output, string manipulation, vectors, and math operations
> Define a global vector unknown_values to store unique unknown values (variables)
> Create a class Indivisual to represent individual terms in an equation. This class initializes the term by extracting its coefficient, unknown value, and operator sign
> Create a class Each_side to handle terms on each side of an equation. It has functions to split the equation into individual terms and operate on similar terms.
> Create a class Split_side to split a complete equation into left-hand side (LHS) and right-hand side (RHS) terms. It also handles the reorganization of terms based on whether they contain unknowns
> Define global vectors CoeffsMatrix and RHSMatrix to store coefficients of equations and the right-hand side values
> Create a class Matrix_operation to perform matrix-related operations. This class forms the coefficient matrix, calculates determinants, and displays matrices
> In the main function:
a. Prompt the user for the number of equations to solve
b. Read each equation, split it into LHS and RHS, and store them in the equations vector
c. Display the input equations
d. Form the coefficient matrix and calculate the determinant
e. Calculate determinants of variable matrices for each unknown
f. Display the coefficient matrix determinant and the solutions for each unknown
