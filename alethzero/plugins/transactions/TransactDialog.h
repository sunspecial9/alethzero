/*
	This file is part of cpp-ethereum.

	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file Transact.h
 * @author Gav Wood <i@gavwood.com>
 * @date 2015
 */

#pragma once

#include <libdevcore/RLP.h>
#include <libethcore/Common.h>
#include <libethereum/Transaction.h>
#include <QDialog>
#include <QMap>
#include <QList>
#include <libaleth/AlethFace.h>
#include "ZeroFace.h"

namespace Ui { class TransactDialog; }

namespace dev
{

namespace eth { class Client; }
namespace solidity { class CompilerStack; }

namespace aleth
{

namespace zero
{

struct GasRequirements
{
	qint64 neededGas;
	qint64 baseGas;
	qint64 executionGas;
	qint64 refundedGas;
	dev::eth::ExecutionResult er;
};

class TransactDialog: public QDialog
{
	Q_OBJECT

public:
	explicit TransactDialog(zero::ZeroFace* _main);
	~TransactDialog();

	void resetGasPrice();
	void setEnvironment(dev::AddressHash const& _accounts, dev::eth::Client* _eth, NatSpecFace* _natSpecDB);

private slots:
	void on_from_currentIndexChanged(int) { updateNonce(); rejigData(); }
	void on_destination_currentTextChanged(QString);
	void on_value_valueChanged(int) { updateFee(); rejigData(); }
	void on_gas_valueChanged(int) { updateFee(); rejigData(); }
	void on_valueUnits_currentIndexChanged(int) { updateFee(); rejigData(); }
	void on_gasPriceUnits_currentIndexChanged(int) { updateFee(); rejigData(); }
	void on_gasPrice_valueChanged(int) { updateFee(); rejigData(); }
	void on_data_textChanged() { rejigData(); }
	void on_optimize_clicked() { rejigData(); }
	void on_copyUnsigned_clicked();
	void on_send_clicked();
	void on_debug_clicked();
	void on_cancel_clicked() { close(); }

private:
	void rejigData();
	void updateNonce();
	void updateBounds();
	void finaliseBounds();

	dev::Address fromAccount();
	std::pair<dev::Address, bytes> toAccount();
	void updateDestination();
	void updateFee();
	bool isCreation() const;
	dev::u256 fee() const;
	dev::u256 gas() const;
	dev::u256 total() const;
	dev::u256 value() const;
	dev::u256 gasPrice() const;
	dev::Address to() const;
	GasRequirements determineGasRequirements();

	std::string natspecNotice(dev::Address _to, dev::bytes const& _data);
	dev::Secret findSecret(dev::u256 _totalReq) const;

	void timerEvent(QTimerEvent*) override;

	AlethFace* aleth() const { return m_main->aleth(); }
	dev::eth::Client* ethereum() const { return aleth()->ethereum(); }

	Ui::TransactDialog* m_ui = nullptr;

	unsigned m_backupGas = 0;
	dev::bytes m_data;

	dev::AddressHash m_accounts;
	zero::ZeroFace* m_main = nullptr;
	NatSpecFace* m_natSpecDB = nullptr;

	QString m_dataInfo;
	qint64 m_startLowerBound = 0;
	qint64 m_startUpperBound = 0;
	qint64 m_lowerBound = 0;
	qint64 m_upperBound = 0;
	eth::ExecutionResult m_lastGood;
	int m_gasCalcTimer = 0;
};

}
}
}
