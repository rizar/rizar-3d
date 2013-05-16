#include "transform.h"

#include <QDebug>

#include <cstring>


namespace {
    static qreal getCoordinate(QVector3D vector, size_t number) {
        if (number == 0) {
            return vector.x();
        } else if (number == 1) {
            return vector.y();
        } else {
            return vector.z();
        }
    }
}

Transform::Transform() {
   memset(matrix, 0, sizeof(matrix));
}

Transform Transform::doNothing() {
    Transform result;
    for (size_t i = 0; i < 4; i++) {
        result.matrix[i][i] = 1;
    }
    return result;
}

Transform Transform::shift(const QVector3D &onVector) {
    Transform result = Transform::doNothing();
    for (quint64 i = 0; i < 3; i++) {
        result.matrix[3][i] = getCoordinate(onVector, i);
    }
    return result;
}

Transform Transform::reflect(quint8 axeNumber) {
    Transform result = Transform::doNothing();
    result.matrix[axeNumber][axeNumber] = -1;
    return result;
}

Transform Transform::orthogonal(QVector3D toI, QVector3D toJ, QVector3D toK) {
    Transform result = Transform::doNothing();
    result.setColumn(0, toI);
    result.setColumn(1, toJ);
    result.setColumn(2, toK);
    return result;
}

Transform Transform::scale(qreal kx, qreal ky, qreal kz)
{
    Transform result = Transform::doNothing();
    result.matrix[0][0] = kx;
    result.matrix[1][1] = ky;
    result.matrix[2][2] = kz;
    return result;
}

Transform Transform::perspective(qreal zMin) {
    Q_ASSERT(zMin > 1e-9);
    Transform result = Transform::doNothing();
    result.matrix[3][3] = 0;
    result.matrix[2][3] = 1;
    result.matrix[2][2] = 1 / (1 - zMin);
    result.matrix[3][2] = -zMin / (1 - zMin);
    return result;
}

Transform Transform::combine(Transform const& other) {
    // just matrix multiplication
    Transform result;
    for (quint64 i = 0; i < 4; i++) {
        for (quint64 j = 0; j < 4; j++) {
            for (quint64 k = 0; k < 4; k++) {
                result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
            }
        }
    }
    return result;
}

QVector3D Transform::apply(QVector3D const& arg) const {
    qreal given[4] = {arg.x(), arg.y(), arg.z(), 1};
    qreal result [4] = {0, 0, 0, 0};

    for (quint64 i = 0; i < 4; i++) {
        for (quint64 j = 0; j < 4; j++) {
            result[j] += given[i] * matrix[i][j];
        }
    }
    for (quint64 i = 0; i < 3; i++) {
        result[i] /= result[3];
    }
    return QVector3D(result[0], result[1], result[2]);
}

void Transform::setColumn(quint8 j, QVector3D vector)
{
    for (quint8 i = 0; i < 3; i++) {
        matrix[i][j] = getCoordinate(vector, i);
    }
}

QDebug operator<<(QDebug stream, Transform const& transform) {
    stream << "Transform(\n";
    for (quint8 i = 0; i < 4; i++) {
        for (quint8 j = 0; j < 4; j++) {
            if (j) {
                stream << ' ';
            }
            stream << transform.matrix[i][j];
        }
        stream << '\n';
    }
    stream << ")";
    return stream.nospace();
}
