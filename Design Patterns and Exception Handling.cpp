#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <memory>
#include <cctype>
#include <limits>
using namespace std;


class Product {
private:
    int id;
    string name;
    double price;
public:
    Product() : id(0), name(""), price(0.0) {}
    Product(int id, const string& name, double price) : id(id), name(name), price(price) {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }

    void display() const {
        cout << left << setw(10) << id << setw(20) << name << setw(10) << price << endl;
    }
};


class CartItem {
private:
    Product product;
    int quantity;
public:
    CartItem() : product(), quantity(0) {}
    CartItem(Product product, int quantity) : product(product), quantity(quantity) {}

    const Product& getProduct() const { return product; }
    int getQuantity() const { return quantity; }
    void addQuantity(int qty) { quantity += qty; }
    double getSubtotal() const { return product.getPrice() * quantity; }

    void display() const {
        cout << left << setw(10) << product.getId()
             << setw(20) << product.getName()
             << setw(10) << product.getPrice()
             << setw(10) << quantity << endl;
    }
};


class PaymentStrategy {
public:
    virtual ~PaymentStrategy() {}
    virtual void pay(double amount) = 0;
    virtual string getMethod() const = 0;
};

// Concrete Payment Strategies
class CashPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid $" << fixed << setprecision(2) << amount << " using Cash." << endl;
    }
    string getMethod() const override { return "Cash"; }
};

class CardPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid $" << fixed << setprecision(2) << amount << " using Credit / Debit Card." << endl;
    }
    string getMethod() const override { return "Credit / Debit Card"; }
};

class GCashPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid $" << fixed << setprecision(2) << amount << " using GCash." << endl;
    }
    string getMethod() const override { return "GCash"; }
};


class OrderLogger {
private:
    static OrderLogger* instance;
    ofstream logFile;

    OrderLogger() {
        logFile.open("orders.log", ios::app);
    }

public:
    static OrderLogger* getInstance() {
        if (!instance) {
            instance = new OrderLogger();
        }
        return instance;
    }

    void log(int orderId, const string& paymentMethod) {
        if (logFile.is_open()) {
            logFile << "[LOG] -> Order ID: " << orderId
                    << " has been successfully checked out and paid using "
                    << paymentMethod << "." << endl;
        }
    }

    ~OrderLogger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
};
OrderLogger* OrderLogger::instance = nullptr;


const int MAX_CART_ITEMS = 10;
class ShoppingCart {
private:
    CartItem items[MAX_CART_ITEMS];
    int itemCount;

public:
    ShoppingCart() : itemCount(0) {}

    void addItem(const Product& product, int quantity) {
        if (quantity <= 0) {
            throw invalid_argument("Quantity must be greater than zero");
        }

        for (int i = 0; i < itemCount; i++) {
            if (items[i].getProduct().getId() == product.getId()) {
                items[i].addQuantity(quantity);
                return;
            }
        }

        if (itemCount < MAX_CART_ITEMS) {
            items[itemCount++] = CartItem(product, quantity);
        } else {
            throw runtime_error("Cart is full");
        }
    }

    double getTotal() const {
        double total = 0;
        for (int i = 0; i < itemCount; i++) {
            total += items[i].getSubtotal();
        }
        return total;
    }

    bool isEmpty() const { return itemCount == 0; }

    const CartItem* getItems() const { return items; }

    int getItemCount() const { return itemCount; }

    void clear() { itemCount = 0; }

    void display() const {
        if (isEmpty()) {
            cout << "Shopping cart is empty." << endl;
            return;
        }

        cout << left << setw(10) << "Product ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << endl;
        for (int i = 0; i < itemCount; i++) {
            items[i].display();
        }
    }
};


const int MAX_ORDERS = 10;
class Order {
private:
    static int nextId;
    int id;
    CartItem items[MAX_CART_ITEMS];
    int itemCount;
    string paymentMethod;
    double total;

public:
    Order(const ShoppingCart& cart, const PaymentStrategy& payment)
        : id(nextId++), itemCount(cart.getItemCount()), paymentMethod(payment.getMethod()), total(cart.getTotal()) {
        const CartItem* cartItems = cart.getItems();
        for (int i = 0; i < itemCount; i++) {
            items[i] = cartItems[i];
        }
    }

    void display() const {
        cout << "Order ID: " << id << endl;
        cout << "Total Amount: " << fixed << setprecision(2) << total << endl;
        cout << "Payment Method: " << paymentMethod << endl;
        cout << "Order Details:" << endl;
        cout << left << setw(10) << "Product ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << endl;
        for (int i = 0; i < itemCount; i++) {
            items[i].display();
        }
        cout << endl;
    }

    int getId() const { return id; }
    string getPaymentMethod() const { return paymentMethod; }
};
int Order::nextId = 1;


const int MAX_PRODUCTS = 10;
class ECommerceSystem {
private:
    Product products[MAX_PRODUCTS];
    int productCount;
    Order orders[MAX_ORDERS];
    int orderCount;
    ShoppingCart cart;

    void displayProducts() const;
    void viewCart();
    void addToCart();
    void checkout();
    void viewOrders() const;

public:
    ECommerceSystem();
    void run();
};




int main() {
    ECommerceSystem system;
    system.run();
    return 0;
}