#pragma once
#include "Entidad.h"
#include "glut.h"

/**
 * @class Plano
 * @brief Representa un suelo estático (Grid) para tener referencia espacial.
 * * Sigue la convención estricta:
 * - Hereda de Entidad.
 * - Separa su definición en .h y su código en .cpp.
 */
class Plano : public Entidad {
public:
	Plano();
	virtual ~Plano();

	// --- CONTRATO DE ENTIDAD ---
	void iniciar() override;
	void actualizar() override;
	void dibujar() override;
};