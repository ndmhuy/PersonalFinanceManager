//
//  AppController.h
//  PersonalFinanceManager
//
//  Created by Le Tran Hoang Long (M2) on 6/12/25.
//

#ifndef AppController_h
#define AppController_h

#include <string>
#include <iostream>

// Include Utils
#include "../Utils/ArrayList.h"
#include "../Utils/HashMap.h"
#include "../Utils/Date.h"
#include "../Utils/IdGenerator.h"
#include "../Utils/Enums.h"

// Include Models
#include "../Models/Transaction.h"
#include "../Models/Income.h"
#include "../Models/Expense.h"
#include "../Models/Wallet.h"
#include "../Models/Category.h"
#include "../Models/IncomeSource.h"
#include "../Models/RecurringTransaction.h"

class AppController {
private:
    // --- 1. DATABASE (IN-MEMORY) ---
    // Danh sách chính lưu dữ liệu
    ArrayList<Transaction*>* transactions;
    ArrayList<RecurringTransaction*>* recurringTransactions;

    // Map để tra cứu nhanh (ID -> Object) - Dùng cho Logic Check
    HashMap<std::string, Wallet*>* walletsMap;
    HashMap<std::string, Category*>* categoriesMap;
    HashMap<std::string, IncomeSource*>* incomeSourcesMap;

    // List để duyệt tuần tự - Dùng cho hiển thị Menu & Báo cáo
    ArrayList<Wallet*>* walletsList;
    ArrayList<Category*>* categoriesList;
    ArrayList<IncomeSource*>* incomeSourcesList;

    // --- 2. INTERNAL HELPER ---
    // Hàm này chạy ngầm để sinh giao dịch định kỳ
    void processRecurringTransactions();

public:
    // Constructor & Destructor
    AppController();
    ~AppController();

    // --- 3. DATA PERSISTENCE ---
    void loadData();
    void saveData();

    // --- 4. WALLET LOGIC ---
    void addWallet(const std::string& name, double initialBalance);
    Wallet* getWalletById(const std::string& id);
    ArrayList<Wallet*>* getWalletsList() const { return walletsList; }
    double getTotalBalance() const;

    // --- 5. MASTER DATA LOGIC ---
    void addCategory(const std::string& name);
    Category* getCategoryById(const std::string& id);
    ArrayList<Category*>* getCategoriesList() const { return categoriesList; }

    void addIncomeSource(const std::string& name);
    IncomeSource* getIncomeSourceById(const std::string& id);
    ArrayList<IncomeSource*>* getIncomeSourcesList() const { return incomeSourcesList; }

    // --- 6. TRANSACTION LOGIC (CORE) ---
    // Thêm giao dịch mới (Tự động trừ/cộng tiền ví)
    void addTransaction(double amount, std::string walletId, std::string categoryOrSourceId, 
                        TransactionType type, Date date, std::string description);
    
    // Xóa giao dịch (Tự động hoàn tiền lại ví)
    bool deleteTransaction(const std::string& transactionId);

    // Lấy toàn bộ danh sách giao dịch
    ArrayList<Transaction*>* getTransactions() const { return transactions; }
    
    // --- 7. RECURRING LOGIC (AUTOMATION) ---
    void addRecurringTransaction(Frequency freq, Date startDate, Date endDate,
                                 std::string walletId, std::string categoryId,
                                 double amount, TransactionType type, std::string desc);

    // --- 8. REPORTING HELPERS ---
    // Lấy danh sách giao dịch trong khoảng thời gian (cho M3 vẽ biểu đồ)
    ArrayList<Transaction*>* getTransactionsByDateRange(Date start, Date end);
};

#endif /* AppController_h */