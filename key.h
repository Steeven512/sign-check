#include <iostream>
#include <sstream>
#include <string>
#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/oids.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
using namespace CryptoPP;

ECDSA<ECP, CryptoPP::SHA256>::PublicKey loadPublicKey(const std::string &pt)
{
 
    HexDecoder decoder;
    decoder.Put((CryptoPP::byte *)&pt[0], pt.size());
    decoder.MessageEnd();
    ECP::Point q;
    size_t len = decoder.MaxRetrievable();
    q.identity = false;
    q.x.Decode(decoder, len / 2);
    q.y.Decode(decoder, len / 2);
    CryptoPP::ECDSA<ECP, CryptoPP::SHA256>::PublicKey publicKey;
    publicKey.Initialize(ASN1::secp256k1(), q);

    //    Integer x = publicKey.GetPublicElement().x;
    //    Integer y = publicKey.GetPublicElement().y;
    //   std::string xStr, yStr;
    //   x.Encode(HexEncoder(new StringSink(xStr)).Ref(), x.MinEncodedSize());
    //    y.Encode(HexEncoder(new StringSink(yStr)).Ref(), y.MinEncodedSize());

    // Imprimir las coordenadas x e y en hexadecimal
    // std::cout << "Coordenada x: " << xStr << std::endl;
    // std::cout << "Coordenada y: " << yStr << std::endl;

    return publicKey;
}

bool verifySignature(const std::string &message, const std::string &signature_hex, const CryptoPP::ECDSA<ECP, CryptoPP::SHA256>::PublicKey &publicKey)
{
    // Conversión a bytes
    std::string signature_str;
    StringSource(signature_hex, true, new HexDecoder(new StringSink(signature_str)));
    const CryptoPP::byte *signature = reinterpret_cast<const CryptoPP::byte *>(signature_str.data());
    size_t signature_size = signature_str.size();
    // Verificación de la firma digital
    CryptoPP::ECDSA<ECP, CryptoPP::SHA256>::Verifier verifier(publicKey);

    bool result = verifier.VerifyMessage((const CryptoPP::byte *)&message[0], message.size(), signature, signature_size);

    if (!result)
    {
        return false;
    }
    return result;
}

std::string derivate(std::string priv)
{

    HexDecoder decoder;
    decoder.Put((CryptoPP::byte *)&priv[0], priv.size());
    decoder.MessageEnd();
    Integer x;

    x.Decode(decoder, decoder.MaxRetrievable());

    CryptoPP::ECDSA<ECP, CryptoPP::SHA256>::PrivateKey k2;
 
 

    k2.Initialize(ASN1::secp256k1(), x);
    CryptoPP::ECDSA<ECP, CryptoPP::SHA256>::PublicKey publicKey;
    k2.MakePublicKey(publicKey);
    std::ostringstream oss;

    HexEncoder encoder2(new FileSink(oss));
    publicKey.DEREncodePublicKey(encoder2);
    std::string output = oss.str();
    

    memset(&publicKey, 0, sizeof(publicKey));
    memset(&k2, 0, sizeof(k2));
    memset(&x, 0, sizeof(x));
    memset(&decoder, 0, sizeof(decoder));
    memset(&priv, 0, sizeof(priv));
    return output;
    
}

std::string Signer(std::string priv, std::string message)
{

    HexDecoder decoder;
    decoder.Put((CryptoPP::byte *)&priv[0], priv.size());
    decoder.MessageEnd();

    Integer x;

    x.Decode(decoder, decoder.MaxRetrievable());

    CryptoPP::ECDSA<ECP, CryptoPP::SHA256>::PrivateKey k2;
    k2.Initialize(ASN1::secp256k1(), x);

    CryptoPP::ECDSA<ECP, CryptoPP::SHA256>::PublicKey publicKey;

    k2.MakePublicKey(publicKey);

    x = publicKey.GetPublicElement().x;
    Integer y = publicKey.GetPublicElement().y;
    std::string xStr, yStr;
    x.Encode(HexEncoder(new StringSink(xStr)).Ref(), x.MinEncodedSize());
    y.Encode(HexEncoder(new StringSink(yStr)).Ref(), y.MinEncodedSize());
/*
    // Imprimir las coordenadas x e y en hexadecimal
    std::cout << "Coordenada x: " << xStr << std::endl;
    std::cout << "Coordenada y: " << yStr << std::endl;
*/
    CryptoPP::ECDSA<ECP, CryptoPP::SHA256>::Signer signer(k2);

    size_t siglen = signer.MaxSignatureLength();
    std::string signature(siglen, 0x00);

    AutoSeededRandomPool prng;
    siglen = signer.SignMessage(prng, (const CryptoPP::byte *)&message[0], message.size(), (CryptoPP::byte *)&signature[0]);

    signature.resize(siglen);

    std::string signature_hex;

    StringSource((const CryptoPP::byte *)&signature[0], signature.size(), true, new HexEncoder(new StringSink(signature_hex)));

    bool lastok = verifySignature(message, signature_hex, publicKey);

    if (!lastok)
    {
        memset(&prng, 0, sizeof(prng));
        memset(&signer, 0, sizeof(signer));
        memset(&publicKey, 0, sizeof(publicKey));
        memset(&k2, 0, sizeof(k2));
        memset(&x, 0, sizeof(x));
        memset(&decoder, 0, sizeof(decoder));
        memset(&priv, 0, sizeof(priv));
        std::cout << "FAIL :(" << std::endl;
    }
    else
    {
        /*
        std::cout << "OK :)" << std::endl;

        std::cout << "la firma es " + signature_hex << std::endl;
        */
        memset(&prng, 0, sizeof(prng));
        memset(&signer, 0, sizeof(signer));
        memset(&publicKey, 0, sizeof(publicKey));
        memset(&k2, 0, sizeof(k2));
        memset(&x, 0, sizeof(x));
        memset(&decoder, 0, sizeof(decoder));
        memset(&priv, 0, sizeof(priv));
        
        return signature_hex;
    }

    return "signing error";
}


