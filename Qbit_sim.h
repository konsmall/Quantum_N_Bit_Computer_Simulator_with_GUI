/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

 /*
  * File:   Qbit_sim.h
  * Author: konma
  *
  * Created on March 12, 2021, 6:19 PM
  */

#pragma once

#include <iostream>
#include <complex>
#include <vector>
#include <math.h>


using namespace std;
typedef vector< complex<double> > line;
typedef vector< vector< complex<double> > > matrix;
typedef vector<vector<int> > gate_table;
#define Had 0.70710678118
#define PI 3.14159265

enum table { I, H, CNOT, X, Y, Z, phase };

class Gates {
private:
    matrix I;
    matrix H;
    matrix CNOT;
    matrix X;
    matrix Y;
    matrix Z;
    matrix phase;

    vector<double> angles;
    int angle_counter = 0;
public:
    void add_angle(double angle) { angles.push_back(angle); }

    Gates();
    matrix gate_I() { return I; }
    matrix gate_H() { return H; }
    matrix gate_CNOT() { return CNOT; }
    matrix gate_X() { return X; }
    matrix gate_Y() { return Y; }
    matrix gate_Z() { return Z; }
    matrix gate_phase( double angle );
};

//matrix tensor_product(matrix A, matrix B);

class Q_sim {
private:
    int num_of_qbits;
    int num_of_steps;

    line Q_bits;
    gate_table Q_table;
    vector< vector< double > > Q_table_angles;

    matrix step_tensor_product(vector<int> gates_vec, vector<double> gates_angles_vec);
    matrix tensor_product(matrix A, matrix B);
    line matrix_product(matrix A, line B);
    matrix line_to_matrix(line ln);
    line convert_individual_bits_to_tensor(line ln);
public:
    Gates gate;
    Q_sim() {}
    Q_sim(int qbits, int steps);

    void init(int qbits, int steps);
    void set_table(int bit, int step, int table);
    void set_table(gate_table table);
    void set_table_angles(vector< vector< double > > table);
    void show_table();
    void show_qbits();
    void add_qbit(int bit_pos, double zeroR, double zeroI, double oneR, double oneI);
    int str_gate_to_enum_gate( string gate_name );
    line run(bool ind_Qbits = true);
};