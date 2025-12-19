//
//  RecurringTransaction.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#include "Models/Transaction.h"
#include "Models/Income.h"
#include "Models/Expense.h"
#include "Models/RecurringTransaction.h"
#include "Utils/BinaryFileHelper.h"
#include <sstream>
#include <iomanip>
#include <ctime>

// ==========================================
// 1. CONSTRUCTORS
// ==========================================

RecurringTransaction::RecurringTransaction()
    : amount(0.0), type(TransactionType::Expense), frequency(Frequency::Monthly) {
}

RecurringTransaction::RecurringTransaction(std::string id, Frequency freq, Date start, Date end, std::string walletId, std::string catId, double amount, TransactionType type, std::string desc)
    : id(id), frequency(freq), startDate(start), endDate(end), walletId(walletId), categoryID(catId), amount(amount), type(type), description(desc) {
}

RecurringTransaction::~RecurringTransaction() = default;

// ==========================================
// 2. GETTERS
// ==========================================

std::string RecurringTransaction::GetId() const { return id; }
Frequency RecurringTransaction::GetFrequency() const { return frequency; }
Date RecurringTransaction::GetStartDate() const { return startDate; }
Date RecurringTransaction::GetEndDate() const { return endDate; }
Date RecurringTransaction::GetLastGeneratedDate() const { return lastGeneratedDate; }

std::string RecurringTransaction::GetWalletId() const { return walletId; }
std::string RecurringTransaction::GetCategoryId() const { return categoryID; }
double RecurringTransaction::GetAmount() const { return amount; }
TransactionType RecurringTransaction::GetType() const { return type; }
std::string RecurringTransaction::GetDescription() const { return description; }

// ==========================================
// 3. SETTERS
// ==========================================

void RecurringTransaction::SetFrequency(Frequency f) { frequency = f; }
void RecurringTransaction::SetStartDate(const Date& d) { startDate = d; }
void RecurringTransaction::SetEndDate(const Date& d) { endDate = d; }
void RecurringTransaction::SetLastGeneratedDate(const Date& d) { lastGeneratedDate = d; }

void RecurringTransaction::SetWalletId(const std::string& w) { walletId = w; }
void RecurringTransaction::SetCategoryId(const std::string& c) { categoryID = c; }
void RecurringTransaction::SetAmount(double a) { amount = a; }
void RecurringTransaction::SetDescription(const std::string& d) { description = d; }

// ==========================================
// 4. AUTOMATION LOGIC (NÂNG CẤP)
// ==========================================

bool RecurringTransaction::ShouldGenerate(const Date& currentDate) {
    // 1. Check End Date
    if (endDate.IsValid() && currentDate > endDate) return false;
    
    // 2. Check Start Date
    if (currentDate < startDate) return false;
    
    // 3. First time run? -> Luôn chạy nếu chưa chạy lần nào
    if (!lastGeneratedDate.IsValid()) return true;
    
    // 4. CHECK FREQUENCY LOGIC (Dựa trên Enums.h)
    // Tính toán "Ngày đến hạn tiếp theo" (Next Due Date) dựa trên lần chạy cuối
    
    int lastD = lastGeneratedDate.GetDay();
    int lastM = lastGeneratedDate.GetMonth();
    int lastY = lastGeneratedDate.GetYear();
    
    Date nextDueDate; // Ngày lẽ ra phải chạy tiếp theo

    switch (frequency) {
        case Frequency::Daily:
            // Đơn giản là ngày hôm sau
            // Logic sơ bộ: Chỉ cần currentDate > lastGeneratedDate là đủ cho Daily
            return lastGeneratedDate < currentDate;

        case Frequency::Weekly:
            // Cần ít nhất 7 ngày trôi qua
            // Vì class Date chưa có hàm cộng trừ ngày phức tạp, ta dùng logic so sánh đơn giản:
            // Nếu khoảng cách giữa 2 ngày < 7 thì False.
            // Tuy nhiên, để chính xác tuyệt đối cần đổi ra số ngày (Julian Day).
            // Ở mức độ đồ án này, ta có thể chấp nhận việc check ngày đơn giản hoặc 
            // tốt nhất là implement logic cộng ngày:
            
            // Cách đơn giản: Chỉ chạy nếu current Date >= lastGeneratedDate + 7 ngày
            // (Phần này cần logic Date math, tạm thời dùng logic check cơ bản bên dưới)
            break; 

        case Frequency::Monthly:
            // Next Due = Ngày đó của tháng sau
            // Ví dụ: Last = 15/01/2025 -> Next = 15/02/2025
            if (lastM == 12) {
                nextDueDate = Date(lastD, 1, lastY + 1);
            } else {
                // Xử lý ngày cuối tháng (VD: 31/01 -> 28/02)
                int nextM = lastM + 1;
                int maxDayNextMonth = Date::DaysInMonth(nextM, lastY);
                int nextD = (lastD > maxDayNextMonth) ? maxDayNextMonth : lastD;
                nextDueDate = Date(nextD, nextM, lastY);
            }
            // Nếu hôm nay đã là (hoặc vượt qua) ngày đến hạn -> Chạy
            return currentDate >= nextDueDate;

        case Frequency::Yearly:
            // Next Due = Ngày đó của năm sau
            // Xử lý 29/02 năm nhuận -> 28/02 năm thường
            if (lastD == 29 && lastM == 2 && !Date::IsLeapYear(lastY + 1)) {
                nextDueDate = Date(28, 2, lastY + 1);
            } else {
                nextDueDate = Date(lastD, lastM, lastY + 1);
            }
            return currentDate >= nextDueDate;
            
        default:
            return false;
    }

    // Fallback cho Weekly (Logic tạm thời nếu chưa có Date Math xịn)
    if (frequency == Frequency::Weekly) {
        // Đây là một cách check tương đối cho Weekly mà không cần đổi ra số ngày:
        // Nếu khác năm hoặc khác tháng -> Chắc chắn đã qua 1 tuần (trừ phi cuối tháng)
        // Đây là điểm khó nhất nếu Date.h không hỗ trợ cộng trừ ngày.
        // Gợi ý: Bạn nên thêm hàm AddDays vào Date.h hoặc dùng thư viện time.h
        
        // GIẢI PHÁP AN TOÀN NHẤT VỚI CODE HIỆN TẠI:
        // Ta dùng time_t của hệ thống để so sánh khoảng cách 7 ngày (604800 giây)
        struct tm lastTm = {0};
        lastTm.tm_year = lastY - 1900;
        lastTm.tm_mon = lastM - 1;
        lastTm.tm_mday = lastD;
        
        struct tm currTm = {0};
        currTm.tm_year = currentDate.GetYear() - 1900;
        currTm.tm_mon = currentDate.GetMonth() - 1;
        currTm.tm_mday = currentDate.GetDay();
        
        time_t lastTime = mktime(&lastTm);
        time_t currTime = mktime(&currTm);
        
        double secondsDiff = difftime(currTime, lastTime);
        // 7 ngày * 24 giờ * 3600 giây = 604800
        return secondsDiff >= 604800; 
    }

    return false;
}

// ==========================================
// 5. DISPLAY
// ==========================================

std::string RecurringTransaction::ToString() const {
    std::stringstream ss;
    
    ss << "[" << EnumHelper::FrequencyToString(frequency) << "] Start: " << startDate.ToString();
    if (endDate.IsValid()) ss << " | End: " << endDate.ToString();
    else ss << " | Indefinite";
    
    if (lastGeneratedDate.IsValid()) ss << " | Last Generated: " << lastGeneratedDate.ToString();
    
    ss << (type == TransactionType::Income ? " + " : " - ")
       << std::fixed << std::setprecision(2) << amount
       << " | " << description;
    
    return ss.str();
}

void RecurringTransaction::ToBinary(std::ofstream& fout) const {
    // 1. Identity & Schedule
    BinaryFileHelper::WriteString(fout, id);
    BinaryFileHelper::Write<int>(fout, static_cast<int>(frequency));
    BinaryFileHelper::WriteDate(fout, startDate);
    BinaryFileHelper::WriteDate(fout, endDate);
    BinaryFileHelper::WriteDate(fout, lastGeneratedDate);
    
    // 2.Template Data
    BinaryFileHelper::WriteString(fout, walletId);
    BinaryFileHelper::WriteString(fout, categoryID);
    BinaryFileHelper::Write<double>(fout, amount);
    BinaryFileHelper::Write<int>(fout, static_cast<int>(type));
    BinaryFileHelper::WriteString(fout, description);
}

RecurringTransaction* RecurringTransaction::FromBinary(std::ifstream& fin) {
    // 1. Identity & Schedule
    std::string id = BinaryFileHelper::ReadString(fin);
    Frequency freq = static_cast<Frequency>(BinaryFileHelper::Read<int>(fin));
    Date start = BinaryFileHelper::ReadDate(fin);
    Date end = BinaryFileHelper::ReadDate(fin);
    Date lastGen = BinaryFileHelper::ReadDate(fin);
    
    // 2. Template Data
    std::string wId = BinaryFileHelper::ReadString(fin);
    std::string catId = BinaryFileHelper::ReadString(fin);
    double amt = BinaryFileHelper::Read<double>(fin);
    TransactionType type = static_cast<TransactionType>(BinaryFileHelper::Read<int>(fin));
    std::string desc = BinaryFileHelper::ReadString(fin);
    
    // 3. Create Object
    RecurringTransaction* rt = new RecurringTransaction(id, freq, start, end, wId, catId, amt, type, desc);
    rt->SetLastGeneratedDate(lastGen);
    
    return rt;
}
