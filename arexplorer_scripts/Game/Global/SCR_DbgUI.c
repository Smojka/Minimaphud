 class SCR_DbgUI
 {
     static void Matrix(string label, vector matrix[4])
     {
         DbgUI.Text(label + "[0] = " + matrix[0].ToString());
         DbgUI.Text(label + "[1] = " + matrix[1].ToString());
         DbgUI.Text(label + "[2] = " + matrix[2].ToString());
         DbgUI.Text(label + "[3] = " + matrix[3].ToString());
     }
     static void Quat(string label, float quat[4])
     {
         DbgUI.Text(label + "[0] = " + quat[0].ToString());
         DbgUI.Text(label + "[1] = " + quat[1].ToString());
         DbgUI.Text(label + "[2] = " + quat[2].ToString());
         DbgUI.Text(label + "[3] = " + quat[3].ToString());
     }
 };