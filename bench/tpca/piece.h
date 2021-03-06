#pragma once

#include "deptran/piece.h"

namespace rococo {

extern char TPCA_BRANCH[];
extern char TPCA_TELLER[];
extern char TPCA_CUSTOMER[];

#define TPCA_PAYMENT (10)
#define TPCA_PAYMENT_NAME "PAYMENT"

#define TPCA_PAYMENT_1 (11)
#define TPCA_PAYMENT_2 (12)
#define TPCA_PAYMENT_3 (13)

#define TPCA_VAR_X   (100)
#define TPCA_VAR_Y   (200)
#define TPCA_VAR_Z   (300)

class TpcaPiece : public Piece {

public:

    void reg_all();

    void reg_pieces();
};

} // namespace rococo
