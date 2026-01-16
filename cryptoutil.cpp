#include "cryptoutil.h"

// 定义一个固定的密钥
const QString CryptoUtil::KEY = "inklin233@gmail";

QString CryptoUtil::encrypt(const QString &plainText)
{
    // 简单的异或加密实现
    // 1. 转为字节数组
    QByteArray data = plainText.toUtf8();
    QByteArray keyBytes = KEY.toUtf8();
    QByteArray result = data;

    // 2. 逐字节异或
    for(int i = 0; i < data.size(); i++) {
        result[i] = data[i] ^ keyBytes[i % keyBytes.size()];
    }

    // 3. 转为 Base64 存储，防止出现数据库乱码字符
    return result.toBase64();
}

QString CryptoUtil::decrypt(const QString &encryptedText)
{
    // 1. Base64 解码
    QByteArray data = QByteArray::fromBase64(encryptedText.toUtf8());
    QByteArray keyBytes = KEY.toUtf8();
    QByteArray result = data;

    // 2. 再次异或还原
    for(int i = 0; i < data.size(); i++) {
        result[i] = data[i] ^ keyBytes[i % keyBytes.size()];
    }

    return QString::fromUtf8(result);
}
