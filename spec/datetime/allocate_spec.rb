require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime.allocate" do
  it "should be the same as jd" do
    DateTime.allocate.should == DateTime.jd
  end
end
