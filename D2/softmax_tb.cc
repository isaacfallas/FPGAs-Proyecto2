/*
 * Softmax Accelerator Testbench - Design 2
 * FPGAs Project 2
 */

#include "softmax.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>

// Modelo de referencia en software (golden model)
void softmax_golden(float *in, float *out, int size) {
    // Encontrar máximo para estabilidad numérica
    float max_val = in[0];
    for (int i = 1; i < size; i++) {
        if (in[i] > max_val) max_val = in[i];
    }
    
    // Calcular suma de exponenciales (con shift por max)
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        sum += std::exp(in[i] - max_val);
    }
    
    // Calcular softmax normalizado
    for (int i = 0; i < size; i++) {
        out[i] = std::exp(in[i] - max_val) / sum;
    }
}

int main(int argc, char **argv) {
    std::cout << "=== Testbench Softmax Optimizado (D2) ===" << std::endl;
    std::cout << "Tamaño del vector: " << kSize << " elementos" << std::endl;
    
    // Arreglos de prueba
    float input[kSize];
    float output_hw[kSize];
    float output_golden[kSize];
    
    // Inicializar con valores de prueba
    // Usamos valores en un rango razonable para evitar overflow en exp()
    std::cout << "\n--- Inicializando datos de entrada ---" << std::endl;
    for (int i = 0; i < kSize; i++) {
        input[i] = (float)(i - kSize/2) * 0.1f;  // Valores de -5.0 a +5.0 aprox
    }
    
    // Mostrar algunos valores de entrada
    std::cout << "Primeros 10 valores de entrada: ";
    for (int i = 0; i < 10; i++) {
        std::cout << std::fixed << std::setprecision(2) << input[i] << " ";
    }
    std::cout << "..." << std::endl;
    
    // Ejecutar modelo golden (referencia)
    std::cout << "\n--- Ejecutando modelo de referencia ---" << std::endl;
    softmax_golden(input, output_golden, kSize);
    
    // Ejecutar función HLS (hardware)
    std::cout << "--- Ejecutando función HLS (softmax optimizado) ---" << std::endl;
    softmax(input, output_hw);
    
    // Verificar resultados
    std::cout << "\n--- Verificando resultados ---" << std::endl;
    float max_error = 0.0f;
    float sum_hw = 0.0f;
    float sum_golden = 0.0f;
    int errors = 0;
    
    for (int i = 0; i < kSize; i++) {
        float error = std::fabs(output_hw[i] - output_golden[i]);
        if (error > max_error) max_error = error;
        if (error > 1e-4f) {  // Tolerancia para errores de punto flotante
            errors++;
            if (errors <= 5) {  // Mostrar solo primeros 5 errores
                std::cout << "Error en [" << i << "]: HW=" << output_hw[i] 
                          << " vs Golden=" << output_golden[i] 
                          << " (diff=" << error << ")" << std::endl;
            }
        }
        sum_hw += output_hw[i];
        sum_golden += output_golden[i];
    }
    
    // Mostrar algunos valores de salida
    std::cout << "\nPrimeros 10 valores de salida (HW): ";
    for (int i = 0; i < 10; i++) {
        std::cout << std::scientific << std::setprecision(4) << output_hw[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Primeros 10 valores de salida (Golden): ";
    for (int i = 0; i < 10; i++) {
        std::cout << std::scientific << std::setprecision(4) << output_golden[i] << " ";
    }
    std::cout << std::endl;
    
    // Resultados finales
    std::cout << "\n=== Resumen ===" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Suma de salidas HW:     " << sum_hw << " (debe ser ~1.0)" << std::endl;
    std::cout << "Suma de salidas Golden: " << sum_golden << " (debe ser ~1.0)" << std::endl;
    std::cout << "Error máximo: " << std::scientific << max_error << std::endl;
    std::cout << "Errores totales (>1e-4): " << errors << "/" << kSize << std::endl;
    
    // Criterio de éxito
    if (errors == 0 && std::fabs(sum_hw - 1.0f) < 1e-3f) {
        std::cout << "\n*** TEST PASSED ***" << std::endl;
        return 0;
    } else {
        std::cout << "\n*** TEST FAILED ***" << std::endl;
        return 1;
    }
}
