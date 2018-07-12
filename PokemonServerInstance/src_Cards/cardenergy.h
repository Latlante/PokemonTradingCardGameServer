#ifndef CARDENERGY_H
#define CARDENERGY_H

#include "abstractcard.h"

class CardEnergy : public AbstractCard
{
public:
    CardEnergy(unsigned short id, const QString& name, AbstractCard::Enum_element element, unsigned short quantity = 1);
    CardEnergy(const CardEnergy& card);
    ~CardEnergy();
	
    AbstractCard::Enum_typeOfCard type() override;
    QUrl image() override;
    AbstractCard* clone() override;

    AbstractCard::Enum_element element();
    QString elementFormatString();
	void setElement(AbstractCard::Enum_element element);
    unsigned short quantity();
	void setQuantity(unsigned short quantity);

    CardEnergy& operator =(const CardEnergy& source);

private:
	AbstractCard::Enum_element m_element;
	unsigned short m_quantity;
};

#endif // CARDENERGY_H
