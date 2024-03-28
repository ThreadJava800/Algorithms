import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

class SuffixAutomatonNode {
	Map<Character, Integer> next; // Transition to next states based on character
	int length; // Length of the node's substring
	int link; // Suffix link to another state

	SuffixAutomatonNode() {
		next = new HashMap<>();
		length = 0;
		link = -1;
	}
}

public class SuffixAutomaton {
	static Vector<SuffixAutomatonNode> suffixAutomaton;
	static int last; // Index of the last state in the automaton

	// Initialize the suffix automaton
	static void initialize() {
		SuffixAutomatonNode initialNode = new SuffixAutomatonNode();
		suffixAutomaton = new Vector<>();
		suffixAutomaton.add(initialNode);
		last = 0;
	}

	// Extend the automaton with a new character
	static void extendAutomaton(char c) {
		SuffixAutomatonNode newNode = new SuffixAutomatonNode();
		newNode.length = suffixAutomaton.get(last).length + 1;

		int current = last;
		while (current != -1 && !suffixAutomaton.get(current).next.containsKey(c)) {
			suffixAutomaton.get(current).next.put(c, suffixAutomaton.size()); 
		// Create a new state
			current = suffixAutomaton.get(current).link;
		}

		if (current == -1) {
			newNode.link = 0; // The root state
		} else {
			int next = suffixAutomaton.get(current).next.get(c);
			if (suffixAutomaton.get(current).length + 1 == suffixAutomaton.get(next).length) {
				newNode.link = next;
			} else {
				SuffixAutomatonNode cloneNode = new SuffixAutomatonNode();
				cloneNode = suffixAutomaton.get(next);
				cloneNode.length = suffixAutomaton.get(current).length + 1;

				suffixAutomaton.add(cloneNode); // Clone the state

				while (current != -1 && suffixAutomaton.get(current).next.get(c) == next) {
					suffixAutomaton.get(current).next.put(c, suffixAutomaton.size() - 1);
					current = suffixAutomaton.get(current).link;
				}

				newNode.link = suffixAutomaton.size() - 1;
				suffixAutomaton.get(next).link = newNode.link;
			}
		}

		suffixAutomaton.add(newNode);
		last = suffixAutomaton.size() - 1;
	}

	// Traverse the suffix automaton
	static void traverseAutomaton() {
		System.out.println("Traversing Suffix Automaton:");
		for (int i = 0; i < suffixAutomaton.size(); ++i) {
			System.out.println("State " + i + ", Length: " + 
							suffixAutomaton.get(i).length +
							", Suffix Link: " + 
							suffixAutomaton.get(i).link);
			for (Map.Entry<Character, Integer> transition : 
				suffixAutomaton.get(i).next.entrySet()) {
				System.out.println(" Transition on '" + 
								transition.getKey() + "' to State " + 
								transition.getValue());
			}
		}
	}

	public static void main(String[] args) {
		String input = "abab";
		initialize();

		for (char c : input.toCharArray()) {
			extendAutomaton(c);
		}

		// Traverse the constructed suffix automaton
		traverseAutomaton();
	}
}
