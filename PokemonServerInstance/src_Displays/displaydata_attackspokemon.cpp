#include "displaydata_attackspokemon.h"

#include "src_Log/log.h"
#include "../Share/constantesshared.h"

DisplayData_AttacksPokemon::DisplayData_AttacksPokemon(const QString &namePlayer, int idPokemon, bool retreatEnable) :
    AbstractDisplayData(namePlayer),
    m_idPokemon(idPokemon),
    m_retreatEnable(retreatEnable)
{

}

/************************************************************
*****               FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonDocument DisplayData_AttacksPokemon::messageInfoToClient()
{
    QJsonObject jsonToReturn;

    jsonToReturn["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifDisplayAttacksPokemon);
    jsonToReturn["idPokemon"] = m_idPokemon;
    jsonToReturn["retreatEnable"] = m_retreatEnable;

    return QJsonDocument(jsonToReturn);
}

bool DisplayData_AttacksPokemon::messageResponseFromClient(const QJsonDocument &document)
{
    if(document.isObject())
    {
        QJsonObject objJson = document.object();
        if(objJson.contains("indexAttack"))
        {
            int indexAttack = objJson["indexAttack"].toInt(-1);
            if(indexAttack >= 0)
            {
                m_argument = QVariant::fromValue(indexAttack);
                m_success = true;
            }
            else
                m_error = "indexAttack " + QString::number(indexAttack) + " is not a good value";
        }
        else
            m_error = "data \"indexAttack\" is missing";
    }
    else
        m_error = "document json is not an object";

    return m_success;
}

QJsonObject DisplayData_AttacksPokemon::messageResultToClient()
{
    QJsonObject jsonToReturn;

    jsonToReturn["phase"] = static_cast<int>(ConstantesShared::PHASE_DisplayAttakcsPokemonResponse);

    if(m_success)
        jsonToReturn["success"] = "ok";
    else
    {
        jsonToReturn["success"] = "ok";
        jsonToReturn["error"] = m_error;
    }

    return jsonToReturn;
}
