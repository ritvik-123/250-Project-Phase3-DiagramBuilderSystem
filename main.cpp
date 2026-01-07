#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <stack>
using namespace std;

// Observer Pattern - Interface
class DrawSubscriber {
public:
    virtual void notify(const string& message) = 0;
};

// Observer Pattern - Concrete Observers
class RegSub : public DrawSubscriber {
public:
    void notify(const string& message) override {
        cout << "[Regular Subscriber] " << message << endl;
    }
};

class ContrastImageSub : public DrawSubscriber {
public:
    void notify(const string& message) override {
        cout << "[Contrast Image Subscriber] " << message << endl;
    }
};

class DiagramVisitor;

// Base class for all Diagrams (Graphs and Figures)
class Diagram {
public:
    virtual void calc() = 0;
    virtual void draw() = 0;
    virtual void drag() = 0;
    virtual void attachSubscriber(shared_ptr<DrawSubscriber> sub) = 0;
    virtual void accept(DiagramVisitor* visitor) = 0;
    virtual ~Diagram() = default;
};

// Visitor Pattern - Interface
class DiagramVisitor {
public:
    virtual void visit(class Graph* g) = 0;
    virtual void visit(class Figure* f) = 0;
    virtual ~DiagramVisitor() = default;
};

// Visitor Pattern - Concrete Visitors for Exporting
class ExportVisitor : public DiagramVisitor {
public:
    void visit(Graph* g) override {
        cout << "Exporting Graph as PNG...\n";
    }
    void visit(Figure* f) override {
        cout << "Exporting Figure as JPG...\n";
    }
};

// Subject in Observer Pattern, Concrete Element in Visitor Pattern
class Graph : public Diagram {
    vector<shared_ptr<DrawSubscriber>> subscribers;
public:
    void calc() override {
        cout << "Calculating Graph\n";
        notifySubscribers("Graph calculated");
    }
    void draw() override {
        cout << "[Graph] Drawing graphical representation.\n";
        notifySubscribers("Graph drawn");
    }
    void drag() override {
        cout << "Dragging Graph\n";
        notifySubscribers("Graph dragged");
    }
    void attachSubscriber(shared_ptr<DrawSubscriber> sub) override {
        subscribers.push_back(sub);
    }
    void accept(DiagramVisitor* visitor) override {
        visitor->visit(this);
    }
private:
    void notifySubscribers(const string& msg) {
        for (auto& s : subscribers) s->notify(msg);
    }
};

// Subject in Observer Pattern, Concrete Element in Visitor Pattern
class Figure : public Diagram {
    vector<shared_ptr<DrawSubscriber>> subscribers;
public:
    void calc() override {
        cout << "Calculating Figure\n";
        notifySubscribers("Figure calculated");
    }
    void draw() override {
        cout << "[Figure Stub] Drawing textual stub.\n";
        notifySubscribers("Figure drawn");
    }
    void drag() override {
        cout << "Dragging Figure\n";
        notifySubscribers("Figure dragged");
    }
    void attachSubscriber(shared_ptr<DrawSubscriber> sub) override {
        subscribers.push_back(sub);
    }
    void accept(DiagramVisitor* visitor) override {
        visitor->visit(this);
    }
private:
    void notifySubscribers(const string& msg) {
        for (auto& s : subscribers) s->notify(msg);
    }
};

// Proxy Pattern - Interface
class DrawProxy {
public:
    virtual void draw() = 0;
};

// Proxy Pattern - Concrete Proxy
class DrawGraph : public DrawProxy {
public:
    void draw() override {
        cout << "[Graph Proxy] Drawing graphical + textual stub\n";
    }
};

// Flyweight Pattern - Abstract Flyweight
class FlyweightFigure {
protected:
    string type;
public:
    FlyweightFigure(string t) : type(t) {}
    virtual void draw() = 0;
    virtual void attachSubscriber(shared_ptr<DrawSubscriber> sub) = 0;
    virtual ~FlyweightFigure() = default;
};

// Flyweight Pattern - Concrete Flyweights
class ColoredFigure : public FlyweightFigure {
    vector<shared_ptr<DrawSubscriber>> subscribers;
public:
    ColoredFigure(string t) : FlyweightFigure(t) {}
    void draw() override {
        cout << "[Colored Flyweight] Drawing colored figure of type: " << type << "\n";
        notifySubscribers("Colored Figure drawn");
    }
    void attachSubscriber(shared_ptr<DrawSubscriber> sub) override {
        subscribers.push_back(sub);
    }
private:
    void notifySubscribers(const string& msg) {
        for (auto& s : subscribers) s->notify(msg);
    }
};

class BWFigure : public FlyweightFigure {
    vector<shared_ptr<DrawSubscriber>> subscribers;
public:
    BWFigure(string t) : FlyweightFigure(t) {}
    void draw() override {
        cout << "[B/W Flyweight] Drawing black and white figure of type: " << type << "\n";
        notifySubscribers("B/W Figure drawn");
    }
    void attachSubscriber(shared_ptr<DrawSubscriber> sub) override {
        subscribers.push_back(sub);
    }
private:
    void notifySubscribers(const string& msg) {
        for (auto& s : subscribers) s->notify(msg);
    }
};

// Flyweight Pattern - Factory
class FlyweightFactory {
    map<string, shared_ptr<FlyweightFigure>> pool;
public:
    shared_ptr<FlyweightFigure> getFigure(string type) {
        if (pool.find(type) == pool.end()) {
            if (type.find("Color") != string::npos)
                pool[type] = make_shared<ColoredFigure>(type);
            else
                pool[type] = make_shared<BWFigure>(type);
        }
        return pool[type];
    }
};

// Builder Pattern - Interface
class Builder {
public:
    virtual void setCoord(string coord) = 0;
    virtual void calc() = 0;
    virtual void draw() = 0;
    virtual void drag() = 0;
    virtual ~Builder() = default;
};

// Builder Pattern - Concrete Builder
class BarBuilder : public Builder {
    string coord;
    DrawGraph proxy;
    BarBuilder() = default;
    ~BarBuilder() = default;
public:
    static BarBuilder& getInstance() {
        static BarBuilder instance;
        return instance;
    }
    void setCoord(string c) override { coord = c; }
    void calc() override { cout << "Bar calc at " << coord << "\n"; }
    void draw() override { proxy.draw(); }
    void drag() override { cout << "Drag Bar at " << coord << "\n"; }
};

// Builder Pattern - Concrete Builder
class LineBuilder : public Builder {
    string coord;
    DrawGraph proxy;
    LineBuilder() = default;
    ~LineBuilder() = default;
public:
    static LineBuilder& getInstance() {
        static LineBuilder instance;
        return instance;
    }
    void setCoord(string c) override { coord = c; }
    void calc() override { cout << "Line calc at " << coord << "\n"; }
    void draw() override { proxy.draw(); }
    void drag() override { cout << "Drag Line at " << coord << "\n"; }
};

// Builder Pattern - Director
class Director {
    Builder* builder;
public:
    void setBuilder(Builder* b) { builder = b; }
    void construct(string type, string coord) {
        builder->setCoord(coord);
        builder->calc();
        builder->draw();
        builder->drag();
    }
};

// Factory Pattern - For creating Graphs
class GraphFactory {
public:
    void createGraph(string type, string coord) {
        Director d;
        if (type == "Bar") {
            d.setBuilder(&BarBuilder::getInstance());
            d.construct(type, coord);
        } else if (type == "Line") {
            d.setBuilder(&LineBuilder::getInstance());
            d.construct(type, coord);
        }
    }
};

// Factory Pattern - Singleton for Figures
class FigureFactory {
    FlyweightFactory flyFactory;
    FigureFactory() = default;
public:
    static FigureFactory& getInstance() {
        static FigureFactory instance;
        return instance;
    }
    shared_ptr<FlyweightFigure> getFigure(string type, string coord, shared_ptr<DrawSubscriber> sub) {
        auto fig = flyFactory.getFigure(type);
        fig->attachSubscriber(sub);
        cout << "Coordinates: " << coord << endl;
        fig->draw();
        return fig;
    }
};

// Command Pattern - Abstract Command
class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() = default;
};

// Command Pattern - Concrete Command
class CreateGraphCommand : public Command {
    string type, coord;
    GraphFactory* factory;
public:
    CreateGraphCommand(GraphFactory* f, string t, string c) : factory(f), type(t), coord(c) {}
    void execute() override {
        factory->createGraph(type, coord);
    }
    void undo() override {
        cout << "Undo creation of graph: " << type << "\n";
    }
};

// Command Pattern - Undo Manager
class Undo {
    stack<shared_ptr<Command>> undoStack;
public:
    void addCommand(shared_ptr<Command> cmd) {
        undoStack.push(cmd);
    }
    shared_ptr<Command> popCommand() {
        if (undoStack.empty()) return nullptr;
        auto cmd = undoStack.top();
        undoStack.pop();
        return cmd;
    }
};

// Command Pattern - Redo Manager
class Redo {
    stack<shared_ptr<Command>> redoStack;
public:
    void addCommand(shared_ptr<Command> cmd) {
        redoStack.push(cmd);
    }
    shared_ptr<Command> popCommand() {
        if (redoStack.empty()) return nullptr;
        auto cmd = redoStack.top();
        redoStack.pop();
        return cmd;
    }
    void clear() {
        while (!redoStack.empty()) redoStack.pop();
    }
};

// High-level Factory - Coordinates command execution, undo/redo, and observers
class DiagramFactory {
    GraphFactory graphFactory;
    Undo undoManager;
    Redo redoManager;
    shared_ptr<DrawSubscriber> regSub = make_shared<RegSub>();
    shared_ptr<DrawSubscriber> contrastSub = make_shared<ContrastImageSub>();
public:
    void createGraph(string type, string coord) {
        auto cmd = make_shared<CreateGraphCommand>(&graphFactory, type, coord);
        cmd->execute();
        undoManager.addCommand(cmd);
        redoManager.clear();
    }
    void createFigure(string type, string coord) {
        auto fig = FigureFactory::getInstance().getFigure(type, coord, regSub);
        fig->attachSubscriber(contrastSub);
    }
    void getDiagram(string element, string type, string coord) {
        if (element == "Graph") createGraph(type, coord);
        else if (element == "Figure") createFigure(type, coord);
    }
    void undo() {
        auto cmd = undoManager.popCommand();
        if (cmd) {
            cmd->undo();
            redoManager.addCommand(cmd);
        }
    }
    void redo() {
        auto cmd = redoManager.popCommand();
        if (cmd) {
            cmd->execute();
            undoManager.addCommand(cmd);
        }
    }
};

int main() {
    DiagramFactory df;

    df.getDiagram("Graph", "Line", "(10,20)");
    df.getDiagram("Graph", "Bar", "(15,30)");
    df.getDiagram("Figure", "CircleColor", "(5,5)");
    df.getDiagram("Figure", "SquareBW", "(2,3)");

    df.undo();
    df.redo();

    ExportVisitor exporter;
    Graph g;
    g.accept(&exporter);
    Figure f;
    f.accept(&exporter);

    return 0;
}
