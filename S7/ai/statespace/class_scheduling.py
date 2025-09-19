import heapq
import itertools
from collections import deque
import random
import time
import math

class Class:
    def __init__(self, id, professor, students):
        self.id = id
        self.professor = professor
        self.students = set(students)
        self.size = len(students)

class Room:
    def __init__(self, id, capacity):
        self.id = id
        self.capacity = capacity

class Assignment:
    def __init__(self, class_id, time_slot, room_id):
        self.class_id = class_id
        self.time_slot = time_slot
        self.room_id = room_id
        
    def __eq__(self, other):
        return (self.class_id == other.class_id and 
                self.time_slot == other.time_slot and 
                self.room_id == other.room_id)
    
    def __hash__(self):
        return hash((self.class_id, self.time_slot, self.room_id))

class CSPState:
    def __init__(self, assignments=None, domains=None):
        self.assignments = assignments if assignments else {}
        self.domains = domains if domains else {}
        self._hash = None
        
    def __lt__(self, other):
        # Define comparison to avoid errors in priority queue
        return id(self) < id(other)
        
    def copy(self):
        return CSPState(self.assignments.copy(), {c: list(d) for c, d in self.domains.items()})
        
    def is_complete(self, classes):
        return len(self.assignments) == len(classes)
    
    def is_consistent(self, classes, professors, student_enrollments, rooms):
        # Check room double-booking
        room_time_assignments = {}
        for class_id, (time_slot, room_id) in self.assignments.items():
            key = (time_slot, room_id)
            if key in room_time_assignments:
                return False
            room_time_assignments[key] = class_id
        
        # Check professor conflicts
        professor_time_assignments = {}
        for class_id, (time_slot, room_id) in self.assignments.items():
            professor = professors[class_id]
            if professor in professor_time_assignments and time_slot in professor_time_assignments[professor]:
                return False
            if professor not in professor_time_assignments:
                professor_time_assignments[professor] = set()
            professor_time_assignments[professor].add(time_slot)
        
        # Check student conflicts
        student_time_assignments = {}
        for class_id, (time_slot, room_id) in self.assignments.items():
            for student in student_enrollments[class_id]:
                if student in student_time_assignments and time_slot in student_time_assignments[student]:
                    return False
                if student not in student_time_assignments:
                    student_time_assignments[student] = set()
                student_time_assignments[student].add(time_slot)
        
        # Check room capacity
        for class_id, (time_slot, room_id) in self.assignments.items():
            if classes[class_id].size > rooms[room_id].capacity:
                return False
                
        return True

class UniversitySchedulingCSP:
    def __init__(self, classes, time_slots, rooms):
        self.classes = {c.id: c for c in classes}
        self.time_slots = time_slots
        self.rooms = {r.id: r for r in rooms}
        
        # Precompute professor and student information
        self.professors = {c.id: c.professor for c in classes}
        self.student_enrollments = {c.id: c.students for c in classes}
        
        # Initialize domains
        self.initial_domains = {}
        for class_id in self.classes:
            self.initial_domains[class_id] = [
                (t, r.id) for t in time_slots for r in rooms 
                if self.classes[class_id].size <= r.capacity
            ]
    
    def get_initial_state(self):
        return CSPState({}, self.initial_domains)
    
    def get_neighbors(self, state):
        """Generate neighboring states by changing one assignment"""
        neighbors = []
        
        # For each assigned class, try changing its time slot or room
        for class_id, (time_slot, room_id) in state.assignments.items():
            for new_time in self.time_slots:
                if new_time != time_slot:
                    new_assignments = state.assignments.copy()
                    new_assignments[class_id] = (new_time, room_id)
                    new_state = CSPState(new_assignments, state.domains)
                    if new_state.is_consistent(self.classes, self.professors, self.student_enrollments, self.rooms):
                        neighbors.append(new_state)
            
            for new_room_id in self.rooms:
                if new_room_id != room_id and self.classes[class_id].size <= self.rooms[new_room_id].capacity:
                    new_assignments = state.assignments.copy()
                    new_assignments[class_id] = (time_slot, new_room_id)
                    new_state = CSPState(new_assignments, state.domains)
                    if new_state.is_consistent(self.classes, self.professors, self.student_enrollments, self.rooms):
                        neighbors.append(new_state)
        
        return neighbors
    
    def count_violations(self, state):
        """Count the number of constraint violations in a state"""
        violations = 0
        
        # Check room double-booking
        room_time_assignments = {}
        for class_id, (time_slot, room_id) in state.assignments.items():
            key = (time_slot, room_id)
            if key in room_time_assignments:
                violations += 1
            room_time_assignments[key] = class_id
        
        # Check professor conflicts
        professor_time_assignments = {}
        for class_id, (time_slot, room_id) in state.assignments.items():
            professor = self.professors[class_id]
            if professor in professor_time_assignments and time_slot in professor_time_assignments[professor]:
                violations += 1
            if professor not in professor_time_assignments:
                professor_time_assignments[professor] = set()
            professor_time_assignments[professor].add(time_slot)
        
        # Check student conflicts
        student_time_assignments = {}
        for class_id, (time_slot, room_id) in state.assignments.items():
            for student in self.student_enrollments[class_id]:
                if student in student_time_assignments and time_slot in student_time_assignments[student]:
                    violations += 1
                if student not in student_time_assignments:
                    student_time_assignments[student] = set()
                student_time_assignments[student].add(time_slot)
        
        # Check room capacity
        for class_id, (time_slot, room_id) in state.assignments.items():
            if self.classes[class_id].size > self.rooms[room_id].capacity:
                violations += 1
                
        return violations

def bfs(csp):
    """Breadth-First Search"""
    queue = deque([csp.get_initial_state()])
    visited = set()
    
    while queue:
        state = queue.popleft()
        
        if state.is_complete(csp.classes):
            return state
            
        state_key = tuple(sorted(state.assignments.items()))
        if state_key in visited:
            continue
        visited.add(state_key)
        
        # Expand next unassigned variable
        unassigned = [c for c in csp.classes if c not in state.assignments]
        if not unassigned:
            continue
            
        next_class = unassigned[0]
        for value in state.domains.get(next_class, csp.initial_domains[next_class]):
            new_assignments = state.assignments.copy()
            new_assignments[next_class] = value
            new_state = CSPState(new_assignments, state.domains)
            
            if new_state.is_consistent(csp.classes, csp.professors, csp.student_enrollments, csp.rooms):
                queue.append(new_state)
    
    return None

def dfs(csp, depth_limit=float('inf')):
    """Depth-First Search with optional depth limit"""
    stack = [(csp.get_initial_state(), 0)]
    visited = set()
    
    while stack:
        state, depth = stack.pop()
        
        if state.is_complete(csp.classes):
            return state
            
        if depth >= depth_limit:
            continue
            
        state_key = tuple(sorted(state.assignments.items()))
        if state_key in visited:
            continue
        visited.add(state_key)
        
        # Expand next unassigned variable
        unassigned = [c for c in csp.classes if c not in state.assignments]
        if not unassigned:
            continue
            
        next_class = unassigned[0]
        for value in state.domains.get(next_class, csp.initial_domains[next_class]):
            new_assignments = state.assignments.copy()
            new_assignments[next_class] = value
            new_state = CSPState(new_assignments, state.domains)
            
            if new_state.is_consistent(csp.classes, csp.professors, csp.student_enrollments, csp.rooms):
                stack.append((new_state, depth + 1))
    
    return None

def ids(csp):
    """Iterative Deepening Search"""
    depth = 0
    while depth <= len(csp.classes):  # Maximum possible depth
        result = dfs(csp, depth)
        if result is not None:
            return result
        depth += 1
    return None

def hill_climbing(csp, max_restarts=100):
    """Hill Climbing with random restarts"""
    best_state = None
    best_violations = float('inf')
    
    for _ in range(max_restarts):
        # Generate a random initial state
        current_state = csp.get_initial_state()
        for class_id in csp.classes:
            if class_id not in current_state.assignments and csp.initial_domains[class_id]:
                current_state.assignments[class_id] = random.choice(csp.initial_domains[class_id])
        
        current_violations = csp.count_violations(current_state)
        
        while True:
            # Check if we found a solution
            if current_violations == 0 and current_state.is_complete(csp.classes):
                return current_state
                
            # Generate all neighbors and find the best one
            neighbors = csp.get_neighbors(current_state)
            if not neighbors:
                break
                
            best_neighbor = None
            best_neighbor_violations = float('inf')
            
            for neighbor in neighbors:
                violations = csp.count_violations(neighbor)
                if violations < best_neighbor_violations:
                    best_neighbor = neighbor
                    best_neighbor_violations = violations
            
            # If no improvement, break out of local optimum
            if best_neighbor_violations >= current_violations:
                break
                
            current_state = best_neighbor
            current_violations = best_neighbor_violations
        
        # Keep track of the best solution found so far
        if current_violations < best_violations:
            best_state = current_state
            best_violations = current_violations
    
    return best_state if best_violations == 0 else None

def best_first_search(csp, heuristic):
    """Best-First Search with a given heuristic"""
    # Use a counter to ensure we always have a tie-breaker
    counter = itertools.count()
    
    # Use negative heuristic value to simulate a priority queue where lower values are better
    queue = []
    initial_state = csp.get_initial_state()
    heapq.heappush(queue, (heuristic(initial_state, csp), next(counter), initial_state))
    visited = set()
    
    while queue:
        _, _, state = heapq.heappop(queue)
        
        if state.is_complete(csp.classes):
            return state
            
        state_key = tuple(sorted(state.assignments.items()))
        if state_key in visited:
            continue
        visited.add(state_key)
        
        # Expand next unassigned variable
        unassigned = [c for c in csp.classes if c not in state.assignments]
        if not unassigned:
            continue
            
        next_class = unassigned[0]
        for value in state.domains.get(next_class, csp.initial_domains[next_class]):
            new_assignments = state.assignments.copy()
            new_assignments[next_class] = value
            new_state = CSPState(new_assignments, state.domains)
            
            if new_state.is_consistent(csp.classes, csp.professors, csp.student_enrollments, csp.rooms):
                heapq.heappush(queue, (heuristic(new_state, csp), next(counter), new_state))
    
    return None

def a_star(csp, heuristic):
    """A* Search with a given heuristic"""
    # Use a counter to ensure we always have a tie-breaker
    counter = itertools.count()
    
    # Use (cost + heuristic) as priority
    queue = []
    initial_state = csp.get_initial_state()
    g_score = 0
    f_score = heuristic(initial_state, csp)
    heapq.heappush(queue, (f_score, next(counter), g_score, initial_state))
    visited = set()
    g_scores = {tuple(sorted(initial_state.assignments.items())): 0}
    
    while queue:
        _, _, cost, state = heapq.heappop(queue)
        
        if state.is_complete(csp.classes):
            return state
            
        state_key = tuple(sorted(state.assignments.items()))
        if state_key in visited:
            continue
        visited.add(state_key)
        
        # Expand next unassigned variable
        unassigned = [c for c in csp.classes if c not in state.assignments]
        if not unassigned:
            continue
            
        next_class = unassigned[0]
        for value in state.domains.get(next_class, csp.initial_domains[next_class]):
            new_assignments = state.assignments.copy()
            new_assignments[next_class] = value
            new_state = CSPState(new_assignments, state.domains)
            
            if new_state.is_consistent(csp.classes, csp.professors, csp.student_enrollments, csp.rooms):
                new_state_key = tuple(sorted(new_state.assignments.items()))
                new_cost = cost + 1
                
                if new_state_key not in g_scores or new_cost < g_scores[new_state_key]:
                    g_scores[new_state_key] = new_cost
                    f_score = new_cost + heuristic(new_state, csp)
                    heapq.heappush(queue, (f_score, next(counter), new_cost, new_state))
    
    return None

def heuristic_constraints(state, csp):
    """Heuristic function: number of constraint violations"""
    return csp.count_violations(state)

def heuristic_remaining_vars(state, csp):
    """Heuristic function: number of unassigned variables"""
    return len(csp.classes) - len(state.assignments)

# For minimax and alpha-beta pruning, we need to define a two-agent game
# Let's create a simplified version where two agents try to schedule classes
# with conflicting preferences (e.g., preferring the same time slots)

class SchedulingGame:
    def __init__(self, csp, agent_classes):
        self.csp = csp
        self.agent_classes = agent_classes  # {agent_id: [class_ids]}
        
    def get_initial_state(self):
        return self.csp.get_initial_state()
        
    def is_terminal(self, state):
        return state.is_complete(self.csp.classes) or not self.get_actions(state)
        
    def get_actions(self, state):
        actions = []
        for class_id in self.csp.classes:
            if class_id not in state.assignments:
                for value in state.domains.get(class_id, self.csp.initial_domains[class_id]):
                    actions.append((class_id, value))
                break  # Only consider one unassigned variable at a time for simplicity
        return actions
        
    def get_result(self, state, action, agent):
        class_id, value = action
        new_assignments = state.assignments.copy()
        new_assignments[class_id] = value
        new_state = CSPState(new_assignments, state.domains)
        return new_state
        
    def get_utility(self, state, agent):
        if not state.is_complete(self.csp.classes):
            return 0
            
        # Utility is based on how many of the agent's preferred time slots are used
        utility = 0
        preferred_times = {"A": ["T1", "T2"], "B": ["T3", "T4"]}  # Example preferences
        
        for class_id in self.agent_classes[agent]:
            if class_id in state.assignments:
                time_slot, room_id = state.assignments[class_id]
                if time_slot in preferred_times[agent]:
                    utility += 1
                    
        return utility

def minimax(game, state, agent, max_agent):
    if game.is_terminal(state):
        return game.get_utility(state, max_agent), None
        
    if agent == max_agent:
        value = -float('inf')
        best_action = None
        for action in game.get_actions(state):
            new_state = game.get_result(state, action, agent)
            new_value, _ = minimax(game, new_state, "B" if agent == "A" else "A", max_agent)
            if new_value > value:
                value = new_value
                best_action = action
        return value, best_action
    else:
        value = float('inf')
        best_action = None
        for action in game.get_actions(state):
            new_state = game.get_result(state, action, agent)
            new_value, _ = minimax(game, new_state, "B" if agent == "A" else "A", max_agent)
            if new_value < value:
                value = new_value
                best_action = action
        return value, best_action

def alpha_beta(game, state, agent, max_agent, alpha=-float('inf'), beta=float('inf')):
    if game.is_terminal(state):
        return game.get_utility(state, max_agent), None
        
    if agent == max_agent:
        value = -float('inf')
        best_action = None
        for action in game.get_actions(state):
            new_state = game.get_result(state, action, agent)
            new_value, _ = alpha_beta(game, new_state, "B" if agent == "A" else "A", max_agent, alpha, beta)
            if new_value > value:
                value = new_value
                best_action = action
            if value >= beta:
                return value, best_action
            alpha = max(alpha, value)
        return value, best_action
    else:
        value = float('inf')
        best_action = None
        for action in game.get_actions(state):
            new_state = game.get_result(state, action, agent)
            new_value, _ = alpha_beta(game, new_state, "B" if agent == "A" else "A", max_agent, alpha, beta)
            if new_value < value:
                value = new_value
                best_action = action
            if value <= alpha:
                return value, best_action
            beta = min(beta, value)
        return value, best_action

# Example usage and test cases
def create_sample_problem():
    # Create sample classes
    classes = [
        Class("C1", "P1", ["S1", "S2", "S3"]),
        Class("C2", "P2", ["S1", "S4", "S5"]),
        Class("C3", "P1", ["S6", "S7", "S8"]),
        Class("C4", "P3", ["S9", "S10", "S2"]),
    ]
    
    # Create time slots
    time_slots = ["T1", "T2", "T3", "T4"]
    
    # Create rooms
    rooms = [
        Room("R1", 5),
        Room("R2", 10),
        Room("R3", 3),
        Room("R4", 8),
    ]
    
    return classes, time_slots, rooms

def print_schedule(state, csp):
    if state is None:
        print("No solution found")
        return
        
    print("Class Schedule:")
    print("Class\tTime\tRoom\tProfessor\tStudents")
    for class_id, (time_slot, room_id) in state.assignments.items():
        cls = csp.classes[class_id]
        print(f"{class_id}\t{time_slot}\t{room_id}\t{cls.professor}\t\t{len(cls.students)} students")

def main():
    # Create a sample problem
    classes, time_slots, rooms = create_sample_problem()
    csp = UniversitySchedulingCSP(classes, time_slots, rooms)
    
    print("Solving University Scheduling Problem")
    print(f"Classes: {len(classes)}, Time Slots: {len(time_slots)}, Rooms: {len(rooms)}")
    print()
    
    # Test BFS
    print("Testing BFS...")
    start_time = time.time()
    solution_bfs = bfs(csp)
    bfs_time = time.time() - start_time
    print(f"BFS completed in {bfs_time:.4f} seconds")
    if solution_bfs:
        print_schedule(solution_bfs, csp)
    else:
        print("No solution found with BFS")
    print()
    
    # Test DFS
    print("Testing DFS...")
    start_time = time.time()
    solution_dfs = dfs(csp)
    dfs_time = time.time() - start_time
    print(f"DFS completed in {dfs_time:.4f} seconds")
    if solution_dfs:
        print_schedule(solution_dfs, csp)
    else:
        print("No solution found with DFS")
    print()
    
    # Test IDFS
    print("Testing IDFS (Iterative Deepening DFS)...")
    start_time = time.time()
    solution_ids = ids(csp)
    ids_time = time.time() - start_time
    print(f"IDFS completed in {ids_time:.4f} seconds")
    if solution_ids:
        print_schedule(solution_ids, csp)
    else:
        print("No solution found with IDFS")
    print()
    
    # Test Hill Climbing
    print("Testing Hill Climbing...")
    start_time = time.time()
    solution_hc = hill_climbing(csp)
    hc_time = time.time() - start_time
    print(f"Hill Climbing completed in {hc_time:.4f} seconds")
    if solution_hc:
        print_schedule(solution_hc, csp)
    else:
        print("No solution found with Hill Climbing")
    print()
    
    # Test Best-First Search
    print("Testing Best-First Search...")
    start_time = time.time()
    solution_bfs = best_first_search(csp, heuristic_constraints)
    best_first_time = time.time() - start_time
    print(f"Best-First Search completed in {best_first_time:.4f} seconds")
    if solution_bfs:
        print_schedule(solution_bfs, csp)
    else:
        print("No solution found with Best-First Search")
    print()
    
    # Test A*
    print("Testing A* Search...")
    start_time = time.time()
    solution_astar = a_star(csp, heuristic_constraints)
    astar_time = time.time() - start_time
    print(f"A* Search completed in {astar_time:.4f} seconds")
    if solution_astar:
        print_schedule(solution_astar, csp)
    else:
        print("No solution found with A* Search")
    print()
    
    # Test Minimax and Alpha-Beta Pruning (two-agent game)
    print("Testing Minimax and Alpha-Beta Pruning...")
    agent_classes = {
        "A": ["C1", "C3"],
        "B": ["C2", "C4"]
    }
    game = SchedulingGame(csp, agent_classes)
    initial_state = game.get_initial_state()
    
    # Minimax
    start_time = time.time()
    value_minimax, action_minimax = minimax(game, initial_state, "A", "A")
    minimax_time = time.time() - start_time
    print(f"Minimax completed in {minimax_time:.4f} seconds")
    print(f"Minimax value: {value_minimax}, action: {action_minimax}")
    
    # Alpha-Beta
    start_time = time.time()
    value_ab, action_ab = alpha_beta(game, initial_state, "A", "A")
    ab_time = time.time() - start_time
    print(f"Alpha-Beta completed in {ab_time:.4f} seconds")
    print(f"Alpha-Beta value: {value_ab}, action: {action_ab}")
    print()

if __name__ == "__main__":
    main()