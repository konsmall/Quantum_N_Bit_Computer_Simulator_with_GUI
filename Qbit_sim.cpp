/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Qbit_sim.h"

 /*---------------GATES---------------*/
Gates::Gates() {
    complex<double> num;

    I.resize(2);
    for (int i = 0; i < I.size(); i++) {
        I[i].resize(2);
    }
    I[0][0] = 1; I[0][1] = 0;
    I[1][0] = 0; I[1][1] = 1;

    H.resize(2);
    for (int i = 0; i < H.size(); i++) {
        H[i].resize(2);
    }
    H[0][0] = Had; H[0][1] = Had;
    H[1][0] = Had; H[1][1] = -Had;

    CNOT.resize(4);
    for (int i = 0; i < CNOT.size(); i++) {
        CNOT[i].resize(4);
    }
    CNOT[0][0] = 1; CNOT[0][1] = 0; CNOT[0][2] = 0; CNOT[0][3] = 0;
    CNOT[1][0] = 0; CNOT[1][1] = 1; CNOT[1][2] = 0; CNOT[1][3] = 0;
    CNOT[2][0] = 0; CNOT[2][1] = 0; CNOT[2][2] = 0; CNOT[2][3] = 1;
    CNOT[3][0] = 0; CNOT[3][1] = 0; CNOT[3][2] = 1; CNOT[3][3] = 0;

    X.resize(2);
    for (int i = 0; i < X.size(); i++) {
        X[i].resize(2);
    }
    X[0][0] = 0; X[0][1] = 1;
    X[1][0] = 1; X[1][1] = 0;

    Y.resize(2);
    for (int i = 0; i < Y.size(); i++) {
        Y[i].resize(2);
    }
    Y[0][0] = 0; Y[0][1] = 0;    Y[0][1].imag(-1);
    Y[1][0] = 0; Y[1][1] = 0;    Y[1][0].imag(1);

    Z.resize(2);
    for (int i = 0; i < Z.size(); i++) {
        Z[i].resize(2);
    }
    Z[0][0] = 1; Z[0][1] = 0;
    Z[1][0] = 0; Z[1][1] = -1;
}

matrix Gates::gate_phase( double angle ) {
    phase.resize(2);
    for (int i = 0; i < phase.size(); i++) {
        phase[i].resize(2);
    }
    phase[0][0] = 1; phase[0][1] = 0;
    phase[1][0] = 0; phase[1][1] = 0;

    phase[1][1].real(cos(angle * PI / 180));
    phase[1][1].imag(sin(angle * PI / 180));
    angle_counter++;

    return phase;
}



/*---------------QSIM---------------*/
Q_sim::Q_sim(int qbits, int steps) {
    num_of_qbits = qbits;
    num_of_steps = steps;

    Q_bits.resize(2 * qbits);
    Q_table.resize(qbits);
    for (int i = 0; i < Q_table.size(); i++) {
        Q_table[i].resize(steps);
        for (int y = 0; y < Q_table[i].size(); y++) {
            Q_table[i][y] = 0;
        }
    }
}

void Q_sim::init(int qbits, int steps) {
    num_of_qbits = qbits;
    num_of_steps = steps;

    Q_bits.resize(2 * qbits);
    Q_table.resize(qbits);
    for (int i = 0; i < Q_table.size(); i++) {
        Q_table[i].resize(steps);
        for (int y = 0; y < Q_table[i].size(); y++) {
            Q_table[i][y] = 0;
        }
    }
}

void Q_sim::set_table(int bit, int step, int table) {
    Q_table[bit][step] = table;
}

void Q_sim::set_table(gate_table table) {
    Q_table = table;
}

void Q_sim::set_table_angles(vector< vector< double > > angles_table) {
    Q_table_angles = angles_table;
}

void Q_sim::show_table() {
    for (int i = 0; i < Q_table.size(); i++) {
        for (int y = 0; y < Q_table[0].size(); y++) {
            cout << "  " << Q_table[i][y];
        }
        cout << "\n";
    }
}

void Q_sim::show_qbits() {
    for (int i = 0; i < Q_bits.size(); i += 2) {
        cout << Q_bits[i] << "" << Q_bits[i + 1];
    }cout << "\n";
}

void Q_sim::add_qbit(int bit_pos, double zeroR, double zeroI, double oneR, double oneI) {
    Q_bits[2 * bit_pos].real(zeroR); Q_bits[2 * bit_pos].imag(zeroI);
    Q_bits[2 * bit_pos + 1].real(oneR); Q_bits[2 * bit_pos + 1].imag(oneI);
}

matrix Q_sim::step_tensor_product(vector<int> gates_vec, vector<double> gates_angles_vec) {
    vector<matrix> mat_vex;
    for (int i = 0; i < gates_vec.size(); i++) {
        if (gates_vec[i] == table::I) {
            mat_vex.push_back(gate.gate_I());
        }
        else if (gates_vec[i] == table::H) {
            mat_vex.push_back(gate.gate_H());
        }
        else if (gates_vec[i] == table::CNOT) {
            mat_vex.push_back(gate.gate_CNOT());
            i++;
        }
        else if (gates_vec[i] == table::X) {
            mat_vex.push_back(gate.gate_X());
        }
        else if (gates_vec[i] == table::Y) {
            mat_vex.push_back(gate.gate_Y());
        }
        else if (gates_vec[i] == table::Z) {
            mat_vex.push_back(gate.gate_Z());
        }
        else if (gates_vec[i] == table::phase) {
            mat_vex.push_back(gate.gate_phase( gates_angles_vec[i] )); //TODO -----> FIX LATER!!!!!!!
        }
    }

    matrix product = mat_vex[0];
    for (int i = 1; i < mat_vex.size(); i++) {
        product = tensor_product(product, mat_vex[i]);
    }

    return product;
}

line Q_sim::matrix_product(matrix A, line B) {
    line product;
    product.resize(B.size());

    complex<double> sum = 0;
    for (int i = 0; i < B.size(); i++) {
        for (int y = 0; y < B.size(); y++) {
            sum = sum + A[i][y] * B[y];
        }
        product[i] = sum;
        sum = 0;
    }

    return product;
}

line Q_sim::run(bool individual_Qbits) {
    line running_bits;
    if (individual_Qbits == true) {
        running_bits = convert_individual_bits_to_tensor(Q_bits);
    }
    else {
        running_bits = Q_bits;
    }
    matrix step_tensor;

    for (int i = 0; i < num_of_steps; i++) {
        vector<int> current_step;
        vector<double> current_step_angles;
        for (int y = 0; y < num_of_qbits; y++) { //TODO MINIMISE TO ONE LINE
            current_step.push_back(Q_table[y][i]);
            current_step_angles.push_back( Q_table_angles[y][i] );
        }

        step_tensor = step_tensor_product(current_step, current_step_angles);
        running_bits = matrix_product(step_tensor, running_bits);
    }

    return running_bits;
}

matrix Q_sim::tensor_product(matrix A, matrix B) {
    int rowa = A.size();
    int cola = A[0].size();
    int rowb = B.size();
    int colb = B[0].size();

    matrix product;
    product.resize(rowa * rowb);
    for (int i = 0; i < product.size(); i++) {
        product[i].resize(cola * colb);
    }

    int rowp = 0; int colp = 0;
    for (int i = 0; i < rowa; i++) {
        for (int k = 0; k < rowb; k++) {
            for (int j = 0; j < cola; j++) {
                for (int l = 0; l < colb; l++) {
                    product[rowp][colp] = A[i][j] * B[k][l];
                    if (colp < cola * colb - 1) {
                        colp += 1;
                    }
                    else {
                        colp = 0;
                    }
                }
            }
            rowp += 1;
        }
    }

    return product;
}

matrix Q_sim::line_to_matrix(line ln) {
    matrix mtrx;
    mtrx.resize(ln.size());
    for (int i = 0; i < mtrx.size(); i++) {
        mtrx[i].resize(1);
        mtrx[i][0] = ln[i];
    }

    return mtrx;
}

line Q_sim::convert_individual_bits_to_tensor(line ln) {
    matrix conv_line_to_matrix = line_to_matrix(ln);
    vector<matrix> matrix_vec;

    /*cout << "a\n";
    for ( int i=0; i<conv_line_to_matrix.size(); i++ ) {
        for ( int y=0; y<conv_line_to_matrix[i].size(); y++ ) {
            cout << " " << conv_line_to_matrix[i][y];
        }cout << "\n";
    }cout << "\n";*/

    for (int i = 0; i < ln.size(); i += 2) {
        matrix temp_mtrx;
        temp_mtrx.resize(2);
        temp_mtrx[0].resize(1); temp_mtrx[1].resize(1);
        temp_mtrx[0][0] = conv_line_to_matrix[i][0];
        temp_mtrx[1][0] = conv_line_to_matrix[i + 1][0];

        matrix_vec.push_back(temp_mtrx);
    }

    /*cout << "b\n";
    for ( int k=0; k<matrix_vec.size(); k++ ) {
        for ( int i=0; i<matrix_vec[k].size(); i++ ) {
            for ( int y=0; y<matrix_vec[k][i].size(); y++ ) {
                cout << " " << matrix_vec[k][i][y];
            }cout << "\n";
        }cout << "\n";
    }cout << "\n";*/

    matrix product;
    product = (matrix_vec[0]);
    for (int i = 1; i < matrix_vec.size(); i++) {
        product = tensor_product(product, matrix_vec[i]);
    }

    line tensor_Q_bits;
    for (int i = 0; i < product.size(); i++) {
        tensor_Q_bits.push_back(product[i][0]);
        cout << " " << tensor_Q_bits[i];
    }cout << endl;

    return tensor_Q_bits;
}

int Q_sim::str_gate_to_enum_gate(string gate_name) {
    if (gate_name == "I") { return table::I; }
    else if (gate_name == "H") { return table::H; }
    else if (gate_name == "CNOT") { return table::CNOT; }
    else if (gate_name == "X") { return table::X; }
    else if (gate_name == "Y") { return table::Y; }
    else if (gate_name == "Z") { return table::Z; }
    else if (gate_name == "phase") { return table::phase; }

    return table::I;
}